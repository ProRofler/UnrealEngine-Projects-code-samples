// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/SKBaseCharacter.h"

#include "Async/Async.h"

#include "Characters/Components/SKCharacterMovementComponent.h"
#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/Components/SKStateMachineComponent.h"

#include "Components/CapsuleComponent.h"

#include "Core/Interface/SKInterfaceCollectible.h"
#include "Core/Interface/SKInterfaceInteractable.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

#include "Gameplay/GAS/Abilities/SKAbilityBase.h"
#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Gameplay/GAS/SKAttributeSet.h"
#include "Gameplay/GAS/SKAttributeSetSkills.h"
#include "Gameplay/GAS/SKCommonGameplayTagsLib.h"

#include "GameplayEffect.h"

ASKBaseCharacter::ASKBaseCharacter(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USKCharacterMovementComponent>(
          ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    // Interactions system
    InteractionZone = CreateDefaultSubobject<UCapsuleComponent>("Interactive zone");
    InteractionZone->SetCapsuleHalfHeight(130.0f);
    InteractionZone->SetCapsuleRadius(100.0f);
    InteractionZone->SetupAttachment(GetRootComponent());
    InteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    InteractionZone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    InteractionZone->SetGenerateOverlapEvents(true);
    InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &ASKBaseCharacter::OnBeginOverlap);
    InteractionZone->OnComponentEndOverlap.AddDynamic(this, &ASKBaseCharacter::OnOverlapEnd);

    // Components
    MovementComponent = Cast<USKCharacterMovementComponent>(GetCharacterMovement());
    InventoryComponent = CreateDefaultSubobject<USKInventoryComponent>("Inventory component");

    // GAS
    AbilitySystemComponent = CreateDefaultSubobject<USKAbilitySystemComponent>("Ability system component");
}

/************************************ UE INHERITED ******************************************/

void ASKBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(InteractionZone);
    check(MovementComponent);
    check(AbilitySystemComponent);

    if (IsValid(AbilitySystemComponent))
    {
        AttributeSet = CastChecked<USKAttributeSet>(AbilitySystemComponent->GetSet<USKAttributeSet>());
        AttributeSetSkills =
            CastChecked<USKAttributeSetSkills>(AbilitySystemComponent->GetSet<USKAttributeSetSkills>());
    }

    TryRunning();

    OnStaminaChanged.BindUObject(this, &ASKBaseCharacter::HandleStaminaChange);
    OnHealthChanged.BindUObject(this, &ASKBaseCharacter::HandleHealthChange);
}

void ASKBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    HandleIdling();
}

void ASKBaseCharacter::OnBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                      UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult &SweepResult)
{
    if (OtherActor->Implements<USKInterfaceInteractable>())
    {
        if (DataGuard.TryWriteLock())
        {
            InteractablesInVicinity.Add(OtherActor);
            DataGuard.WriteUnlock();
        }
        HandleInteractionsTimer();
    }
}

void ASKBaseCharacter::OnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    if (InteractablesInVicinity.Contains(OtherActor))
    {
        if (DataGuard.TryWriteLock())
        {
            InteractablesInVicinity.Remove(OtherActor);
            DataGuard.WriteUnlock();
        }
        InteractionTarget = nullptr;
    }
}

/************************************ Attributes ******************************************/

float ASKBaseCharacter::GetStaminaPercent() const
{
    return AttributeSet->GetStamina() / AttributeSet->GetMaxStamina(); // clamping is already made in attribute set
}

float ASKBaseCharacter::GetHealthPercent() const
{
    return AttributeSet->GetHealth() / AttributeSet->GetMaxHealth();
    // return 0.5f;
}

void ASKBaseCharacter::HandleStaminaChange(const float ChangedAmount)
{
    if (bEnableLogging && bEnableLoggingAbilitySystem)
        UE_LOGFMT(LogSKAbilitySystem, Display, "{Actor} stamina change call for {Amount}",
                  ("Actor", GetOwner()->GetName()), ("Amount", FString::SanitizeFloat(ChangedAmount)));

    if (ChangedAmount < 0)
    {
        AbilitySystemComponent->StopStaminaRegeneration();

        HandleRegenTimer(StaminaRegenTimerHandle, AbilitySystemComponent.Get(),
                         &USKAbilitySystemComponent::StartStaminaRegeneration, 0.2f, 2.0f);

        if (FMath::IsNearlyZero(Cast<USKAttributeSet>(AttributeSet)->GetStamina()))
        {
            HandleStaminaDepleted();
        }
    }
    else
    {
        if (IsStaminaFull() || IsDead())
        {
            GetWorldTimerManager().ClearTimer(StaminaRegenTimerHandle);
            AbilitySystemComponent->StopStaminaRegeneration();
        }
    }
}

void ASKBaseCharacter::HandleHealthChange(const float ChangedAmount)
{
    if (bEnableLogging && bEnableLoggingAbilitySystem)
        UE_LOGFMT(LogSKAbilitySystem, Display, "{Actor} health change call for {Amount}",
                  ("Actor", GetOwner()->GetName()), ("Amount", FString::SanitizeFloat(ChangedAmount)));

    if (ChangedAmount < 0)
    {
        AbilitySystemComponent->StopHealthRegeneration();

        if (IsDead())
        {
            if (HealthRegenTimerHandle.IsValid()) GetWorldTimerManager().ClearTimer(HealthRegenTimerHandle);
            HandleDeath();
            return;
        }

        HandleRegenTimer(HealthRegenTimerHandle, AbilitySystemComponent.Get(),
                         &USKAbilitySystemComponent::StartHealthRegeneration, 0.2f, 2.0f);

        if (FMath::IsNearlyZero(Cast<USKAttributeSet>(AttributeSet)->GetHealth()))
        {
            // TODO: DEATH
            UE_LOGFMT(LogSKAbilitySystem, Display, "{Actor} has died", ("Actor", GetOwner()->GetName()));
        }
    }
    else
    {
        if (IsHeathFull())
        {
            GetWorldTimerManager().ClearTimer(HealthRegenTimerHandle);
            AbilitySystemComponent->StopHealthRegeneration();
        }
    }
}

void ASKBaseCharacter::HandleDeath()
{
    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->Deactivate();

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

void ASKBaseCharacter::HandleStaminaDepleted()
{

    FGameplayTagContainer tagsToCancel;
    tagsToCancel.AddTag(USKCommonGameplayTagsLib::GetTag_Sprinting());

    AbilitySystemComponent->CancelAbilities(&tagsToCancel);
}

bool ASKBaseCharacter::IsStaminaFull() const
{
    return FMath::IsNearlyEqual(AttributeSet->GetStamina(), AttributeSet->GetMaxStamina());
}

bool ASKBaseCharacter::IsHeathFull() const
{
    return FMath::IsNearlyEqual(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
}

bool ASKBaseCharacter::IsDead() const { return FMath::IsNearlyZero(AttributeSet->GetHealth()); }

//************************** GAS **************************
UAbilitySystemComponent *ASKBaseCharacter::GetAbilitySystemComponent() const
{
    return Cast<UAbilitySystemComponent>(AbilitySystemComponent.Get());
}

/************************************ MOVEMENT  ******************************************/

void ASKBaseCharacter::TrySprinting()
{

    if (AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_GrabbingItem())) return;

    const auto wantsToSprintTagContainer = USKCommonGameplayTagsLib::GetTag_WantsToSprint().GetSingleTagContainer();

    if (AbilitySystemComponent->CheckAndAddGameplayTag(USKCommonGameplayTagsLib::GetTag_WantsToSprint()))
    {
        AbilitySystemComponent->TryActivateAbilitiesByTag(wantsToSprintTagContainer);

        if (bEnableLogging && bEnableLoggingAbilitySystem)
            UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' wants to sprint",
                      ("ActorName", this->GetName()));
    }
    else
    {

        AbilitySystemComponent->RemoveLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_WantsToSprint());
        AbilitySystemComponent->CancelAbilities(&wantsToSprintTagContainer);

        if (bEnableLogging && bEnableLoggingAbilitySystem)
            UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' no loger wants to sprint",
                      ("ActorName", this->GetName()));
    }
}

void ASKBaseCharacter::StartSprinting() const
{
    MovementComponent->StartSprinting();
    OnStartedSprinting.Broadcast();
}

void ASKBaseCharacter::TryWalking()
{
    if (AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_GrabbingItem()) ||
        AbilitySystemComponent->HasMatchingGameplayTag(
            FGameplayTag::RequestGameplayTag("Character.Request.Movement.WantsToSprint")))
        return;

    if (AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_Idling()))
    {
        AbilitySystemComponent->RemoveLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_Idling());
    }

    if (!AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_Walking()))
    {
        AbilitySystemComponent->AddLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_Walking());
        MovementComponent->StartWalking();

        if (AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_Running()))
        {
            AbilitySystemComponent->RemoveLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_Running());
        }

        if (bEnableLogging && bEnableLoggingAbilitySystem)
            UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is now walking",
                      ("ActorName", this->GetName()));
    }
    else
    {
        AbilitySystemComponent->RemoveLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_Walking());
        TryRunning();
    }
}

void ASKBaseCharacter::TryJumping()
{

    if (CanJump() && AbilitySystemComponent->CheckAndAddGameplayTag(USKCommonGameplayTagsLib::GetTag_WantsToJump()))
    {
        if (!AbilitySystemComponent->TryActivateAbilitiesByTag(
                USKCommonGameplayTagsLib::GetTag_WantsToJump().GetSingleTagContainer()))

            AbilitySystemComponent->CheckAndRemoveGameplayTag(USKCommonGameplayTagsLib::GetTag_WantsToJump());
    }
}

bool ASKBaseCharacter::IsMovingForward() const
{
    return GetActorForwardVector().Equals(GetVelocity().GetSafeNormal(), 0.6f);
}

void ASKBaseCharacter::Landed(const FHitResult &Hit)
{
    Super::Landed(Hit);
    const auto velocity = GetVelocity().Z;

    AbilitySystemComponent->ApplyFallDamage(velocity);

    if (bEnableLogging)
        UE_LOGFMT(LogSKCharacterMovement, Display, "Actor: {1} has landed with velocity: {2}", ("1", this->GetName()),
                  ("2", velocity));
}

float ASKBaseCharacter::GetCharacterMovementAngle() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto VectorsAngle = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degree = FMath::RadiansToDegrees(VectorsAngle);

    return CrossProduct.IsZero() ? Degree : Degree * FMath::Sign(CrossProduct.Z);
}

void ASKBaseCharacter::TryRunning() const
{
    if (AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_Running())) return;

    AbilitySystemComponent->CheckAndRemoveGameplayTag(USKCommonGameplayTagsLib::GetTag_Walking());
    AbilitySystemComponent->CheckAndRemoveGameplayTag(USKCommonGameplayTagsLib::GetTag_Idling());

    MovementComponent->StartRunning();
    OnStartedRunning.Broadcast();

    if (!AbilitySystemComponent->CheckAndAddGameplayTag(USKCommonGameplayTagsLib::GetTag_Running())) checkNoEntry();

    if (bEnableLogging && bEnableLoggingAbilitySystem)
        UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is now running",
                  ("ActorName", this->GetName()));
}

/************************************ ACTIONS  ******************************************/
void ASKBaseCharacter::TryDrawWeapon()
{
    const auto wantsToDrawWeaponTag = FGameplayTag::RequestGameplayTag("Character.Request.Action.WantsToDrawWeapon");
    const auto wantsToDrawWeaponTagCcontainer = wantsToDrawWeaponTag.GetSingleTagContainer();

    if (!AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_WeaponDrawn()) &&
        !AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_Sprinting()))
    {
        AbilitySystemComponent->CheckAndAddGameplayTag(wantsToDrawWeaponTag);
        AbilitySystemComponent->TryActivateAbilitiesByTag(wantsToDrawWeaponTagCcontainer);
    }
    else
    {
        AbilitySystemComponent->CancelAbilities(&wantsToDrawWeaponTagCcontainer);
    }
}

/************************************ STATE  ******************************************/

bool ASKBaseCharacter::IsCharacterMoving() const
{
    return !GetVelocity().IsNearlyZero() && !MovementComponent->IsFalling();
}

bool ASKBaseCharacter::CanJumpInternal_Implementation() const
{
    const bool isEnoughStamina = GetAbilitySystemComponent()->GetSet<USKAttributeSet>()->GetStamina() > 15.0f;

    return Super::CanJumpInternal_Implementation() && isEnoughStamina;
}

bool ASKBaseCharacter::CanSprint(uint8 RequiredStaminaPercentage) const
{
    if (RequiredStaminaPercentage > 100 || RequiredStaminaPercentage < 1)
    {
        RequiredStaminaPercentage = FMath::Clamp(RequiredStaminaPercentage, 1, 100);
        UE_LOGFMT(LogSKCharacterMovement, Warning,
                  "In {1}'s {2} function provided argumemnt was automatically clamped to fit 1 to 100 range",
                  ("1", GetName()), ("2", GET_FUNCTION_NAME_CHECKED(ASKBaseCharacter, CanSprint)));
    }

    const bool isEnoughStamina = GetStaminaPercent() >= (float)RequiredStaminaPercentage / 100.0f;

    return isEnoughStamina && IsMovingForward() && !GetMovementComponent()->IsFalling();
}

void ASKBaseCharacter::HandleIdling() // adds idle tag and stops sprinting
{
    if (!IsCharacterMoving())
    {
        StartIdle();
    }
    else
    {
        StopIdle();
    }
}

void ASKBaseCharacter::StartIdle()
{
    if (AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_Sprinting()))
    {
        auto sprintingTagContainer = USKCommonGameplayTagsLib::GetTag_Sprinting().GetSingleTagContainer();
        AbilitySystemComponent->CancelAbilities(&sprintingTagContainer);
    }

    if (AbilitySystemComponent->CheckAndAddGameplayTag(USKCommonGameplayTagsLib::GetTag_Idling()))
    {

        if (bEnableLogging && bEnableLoggingAbilitySystem)
            UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is now idling",
                      ("ActorName", this->GetName()));
    }
}

void ASKBaseCharacter::StopIdle()
{
    if (AbilitySystemComponent->CheckAndRemoveGameplayTag(USKCommonGameplayTagsLib::GetTag_Idling()))
    {
        if (bEnableLogging && bEnableLoggingAbilitySystem)
            UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is no longer idling",
                      ("ActorName", this->GetName()));
    }

    if (!IsMovingForward() &&
        AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_Sprinting()))
    {
        auto sprintingTag = USKCommonGameplayTagsLib::GetTag_Sprinting().GetSingleTagContainer();
        AbilitySystemComponent->CancelAbilities(&sprintingTag);
    }
}

//************************** MULTITHREADING **************************
void ASKBaseCharacter::AsyncInteractionHandle()
{
    Async(EAsyncExecution::TaskGraph, [&]() { HandleInteractionActor(); });
}

//************************** INTERACTIONS **************************
void ASKBaseCharacter::HandleInteractionsTimer()
{
    if (!GetWorld()) return;

    if (InteractablesInVicinity.Num() > 0 && GetWorldTimerManager().IsTimerActive(InteractionTimer))
    {
        return;
    }

    else if (InteractablesInVicinity.Num() > 0)
    {
        GetWorldTimerManager().SetTimer(InteractionTimer, this, &ASKBaseCharacter::AsyncInteractionHandle, 0.1, true);
    }

    else
    {
        GetWorldTimerManager().ClearTimer(InteractionTimer);
    }
}

void ASKBaseCharacter::HandleInteractionActor()
{
    // TODO: AI interaction logic
}

void ASKBaseCharacter::Interact()
{
    if (!InteractionTarget.IsValid() || !AbilitySystemComponent->InteractAbility) return;

    const auto canInteractTag = FGameplayTag::RequestGameplayTag("Character.State.Action.CanInteract");
    const auto wantToInteractTag = FGameplayTag::RequestGameplayTag("Character.Request.Action.WantsToInteract");

    if (AbilitySystemComponent->HasMatchingGameplayTag(canInteractTag))
    {
        if (bEnableLogging && bEnableLoggingAbilitySystem)
            UE_LOGFMT(LogSKAbilitySystem, Display, "'{ActorName}' tried to interact with '{InteractableName}'!",
                      ("ActorName", this->GetName()), ("InteractableName", InteractionTarget->GetName()));

        if (AbilitySystemComponent->CheckAndAddGameplayTag(wantToInteractTag))
        {
            if (InteractionTarget->Implements<USKInterfaceCollectible>())
            {
                InventoryComponent->AddToInventory(InteractionTarget.Get());
            }

            if (!AbilitySystemComponent->TryActivateAbilitiesByTag(wantToInteractTag.GetSingleTagContainer()))
            {
                UE_LOGFMT(LogSKAbilitySystem, Error,
                          "Actor '{ActorName}' interact ability was not activated for unknown reasons!",
                          ("ActorName", this->GetName()));
            }
        }
    }
    else
    {
        AbilitySystemComponent->CheckAndRemoveGameplayTag(wantToInteractTag);
    }
}