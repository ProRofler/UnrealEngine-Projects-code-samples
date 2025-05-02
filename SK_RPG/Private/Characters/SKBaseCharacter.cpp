// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/SKBaseCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"

#include "Async/Async.h"

#include "Characters/Components/SKCharacterMovementComponent.h"
#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/Components/SKStateMachineComponent.h"
#include "Characters/Components/SKWeaponComponent.h"

#include "Components/CapsuleComponent.h"

#include "Core/Interface/SKInterfaceCollectible.h"
#include "Core/Interface/SKInterfaceInteractable.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

#include "Gameplay/GAS/Abilities/SKAbilityBase.h"
#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Gameplay/GAS/SKAttributeSetSkills.h"
#include "Gameplay/GAS/SKNativeGameplayTags.h"

#include "Utils/DataAssets/SKAbilitiesDataAsset.h"
#include "Utils/DataAssets/SKBasicGameplayEffectsDataAsset.h"

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
    WeaponComponent = CreateDefaultSubobject<USKWeaponComponent>("Weapon component");

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
    check(BasicGameplayEffects)

        BasicGameplayEffects->ValidateData();

    if (IsValid(AbilitySystemComponent))
    {
        AttributeSet = CastChecked<USKAttributeSet>(AbilitySystemComponent->GetSet<USKAttributeSet>());
        AttributeSetSkills =
            CastChecked<USKAttributeSetSkills>(AbilitySystemComponent->GetSet<USKAttributeSetSkills>());
    }

    checkf(GrantedAbilitiesDataAsset, TEXT("You need to assign abilities data asset"));
    AbilitySystemComponent->AddCharacterAbilities(GrantedAbilitiesDataAsset->ActiveAbilities);
    AbilitySystemComponent->AddCharacterPassiveAbilities(GrantedAbilitiesDataAsset->PassiveAbilities);

    TryRunning();

    OnMainAttributeChanged.BindUObject(this, &ASKBaseCharacter::HandleMainAttributeChange);
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

float ASKBaseCharacter::GetStaminaPercent() const { return AttributeSet->GetStamina() / AttributeSet->GetMaxStamina(); }

float ASKBaseCharacter::GetHealthPercent() const { return AttributeSet->GetHealth() / AttributeSet->GetMaxHealth(); }

void ASKBaseCharacter::HandleMainAttributeChange(const FSKAttributeChangeData ChangedAttributeInfo)
{
    if (bEnableLogging && bEnableLoggingAbilitySystem)
        UE_LOGFMT(LogSKAbilitySystem, Display, "{Actor} {Attribute} change call for {Amount}",
                  ("Actor", GetOwner()->GetName()), ("Attribute", ChangedAttributeInfo.AttributeTag.ToString()),
                  ("Amount", FString::SanitizeFloat(ChangedAttributeInfo.ChangedAmount)));

    FGameplayEventData payloadTemp; // temporary, don't know if I'm gonna use it in future

    if (ChangedAttributeInfo.ChangedAmount <= 0)
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, ChangedAttributeInfo.EventDecreasedTag,
                                                                 payloadTemp);

        if (FMath::IsNearlyZero(ChangedAttributeInfo.CurrentValue))
        {
            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, ChangedAttributeInfo.EventDepletedTag,
                                                                     payloadTemp);
        }
    }
    else
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, ChangedAttributeInfo.EventIncreasedTag,
                                                                 payloadTemp);

        if (FMath::IsNearlyEqual(ChangedAttributeInfo.CurrentValue, ChangedAttributeInfo.MaxValue))
        {
            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, ChangedAttributeInfo.EventFullTag,
                                                                     payloadTemp);
        }
    }
}

void ASKBaseCharacter::HandleDeath()
{

    OnCharacterDeath.Broadcast();

    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->Deactivate();

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
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

void ASKBaseCharacter::TryWalking()
{
    const auto runningTag = FSKGameplayTags::Get().Character_State_Movement_Running;
    const auto walkingTag = FSKGameplayTags::Get().Character_State_Movement_Walking;
    const auto idlingTag = FSKGameplayTags::Get().Character_State_Movement_Idling;
    const auto grabbingItemTag = FSKGameplayTags::Get().Character_State_Action_GrabbingItem;

    if (AbilitySystemComponent->HasMatchingGameplayTag(grabbingItemTag)) return;

    if (AbilitySystemComponent->HasMatchingGameplayTag(idlingTag))
    {
        AbilitySystemComponent->RemoveLooseGameplayTag(idlingTag);
    }

    if (!AbilitySystemComponent->HasMatchingGameplayTag(walkingTag))
    {
        AbilitySystemComponent->AddLooseGameplayTag(walkingTag);
        MovementComponent->StartWalking();

        if (AbilitySystemComponent->HasMatchingGameplayTag(runningTag))
        {
            AbilitySystemComponent->RemoveLooseGameplayTag(runningTag);
        }

        if (bEnableLogging && bEnableLoggingAbilitySystem)
            UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is now walking",
                      ("ActorName", this->GetName()));
    }
    else
    {
        AbilitySystemComponent->RemoveLooseGameplayTag(walkingTag);
        TryRunning();
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
    if (-velocity < BasicGameplayEffects->LandingSpeedRange.X) return;

    // Make handle and apply spec to self
    const auto GESpecHandle = AbilitySystemComponent->MakeGESpecHandle(BasicGameplayEffects->FallDamageGameplayEffect);

    const auto calculatedDamage =
        FMath::GetMappedRangeValueClamped(BasicGameplayEffects->LandingSpeedRange,
                                          BasicGameplayEffects->LandingDamageRange, -velocity) *
        -1.0f;

    GESpecHandle.Data->SetSetByCallerMagnitude(FSKGameplayTags::Get().Effect_Data_FallDamage, calculatedDamage);

    AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data.Get());

    //
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

void ASKBaseCharacter::TrySprinting() const
{
    const auto runningTag = FSKGameplayTags::Get().Character_State_Movement_Running;
    const auto walkingTag = FSKGameplayTags::Get().Character_State_Movement_Walking;

    AbilitySystemComponent->CheckAndRemoveGameplayTag(walkingTag);
    AbilitySystemComponent->CheckAndRemoveGameplayTag(runningTag);

    MovementComponent->StartSprinting();

    if (bEnableLogging && bEnableLoggingAbilitySystem)
        UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is now sprinting",
                  ("ActorName", this->GetName()));
}

void ASKBaseCharacter::TryRunning() const
{
    const auto runningTag = FSKGameplayTags::Get().Character_State_Movement_Running;
    const auto walkingTag = FSKGameplayTags::Get().Character_State_Movement_Walking;
    const auto idlingTag = FSKGameplayTags::Get().Character_State_Movement_Idling;

    if (AbilitySystemComponent->HasMatchingGameplayTag(runningTag)) return;

    AbilitySystemComponent->CheckAndRemoveGameplayTag(walkingTag);
    AbilitySystemComponent->CheckAndRemoveGameplayTag(idlingTag);

    MovementComponent->StartRunning();

    if (!AbilitySystemComponent->CheckAndAddGameplayTag(runningTag)) checkNoEntry();

    if (bEnableLogging && bEnableLoggingAbilitySystem)
        UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is now running",
                  ("ActorName", this->GetName()));
}

/************************************ ACTIONS  ******************************************/

void ASKBaseCharacter::HandleUseItem(USKInventoryObjectData *ObjectData)
{
    // send event

    FGameplayEventData eventPayload;
    eventPayload.OptionalObject = ObjectData;

    AbilitySystemComponent->HandleGameplayEvent(FSKGameplayTags::Get().Event_Ability_UseItem, &eventPayload);
}

/************************************ STATE  ******************************************/

bool ASKBaseCharacter::IsCharacterMoving() const
{
    return !GetVelocity().IsNearlyZero() && !MovementComponent->IsFalling();
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
    const auto idlingTag = FSKGameplayTags::Get().Character_State_Movement_Idling;
    const auto sprintingTag = FSKGameplayTags::Get().Character_State_Movement_Sprinting;

    if (AbilitySystemComponent->HasMatchingGameplayTag(sprintingTag))
    {
        auto sprintingTagContainer = sprintingTag.GetSingleTagContainer();
        AbilitySystemComponent->CancelAbilities(&sprintingTagContainer);
    }

    if (AbilitySystemComponent->CheckAndAddGameplayTag(idlingTag))
    {

        if (bEnableLogging && bEnableLoggingAbilitySystem)
            UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is now idling",
                      ("ActorName", this->GetName()));
    }
}

void ASKBaseCharacter::StopIdle()
{
    const auto idlingTag = FSKGameplayTags::Get().Character_State_Movement_Idling;
    const auto sprintingTag = FSKGameplayTags::Get().Character_State_Movement_Sprinting;

    if (AbilitySystemComponent->CheckAndRemoveGameplayTag(idlingTag))
    {
        if (bEnableLogging && bEnableLoggingAbilitySystem)
            UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is no longer idling",
                      ("ActorName", this->GetName()));
    }

    if (!IsMovingForward() && AbilitySystemComponent->HasMatchingGameplayTag(sprintingTag))
    {
        auto sprintingTagContainer = sprintingTag.GetSingleTagContainer();
        AbilitySystemComponent->CancelAbilities(&sprintingTagContainer);
    }
}

void ASKBaseCharacter::Interact()
{
    if (!InteractionTarget.IsValid()) return;

    if (bEnableLogging && bEnableLoggingAbilitySystem)
        UE_LOGFMT(LogSKAbilitySystem, Display, "'{ActorName}' tried to interact with '{InteractableName}'!",
                  ("ActorName", this->GetName()), ("InteractableName", InteractionTarget->GetName()));

    if (InteractionTarget->Implements<USKInterfaceCollectible>())
    {
        InventoryComponent->AddToInventory(InteractionTarget.Get());
    }

    ISKInterfaceInteractable::Execute_OnInteraction(InteractionTarget.Get(), this);
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

    if (InteractablesInVicinity.Num() > 0 && GetWorldTimerManager().IsTimerActive(InteractableActiveUpdateTimer))
    {
        return;
    }

    else if (InteractablesInVicinity.Num() > 0)
    {
        GetWorldTimerManager().SetTimer(InteractableActiveUpdateTimer, this, &ASKBaseCharacter::AsyncInteractionHandle,
                                        0.1, true);
    }

    else
    {
        GetWorldTimerManager().ClearTimer(InteractableActiveUpdateTimer);
    }
}

void ASKBaseCharacter::HandleInteractionActor()
{
    // Not supposed to be called for now
}
