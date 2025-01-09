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
#include "Gameplay/GAS/Abilities/SKAbilityBase.h"
#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Gameplay/GAS/SKAttributeSet.h"
#include "Gameplay/GAS/SKAttributeSetSkills.h"
#include "Gameplay/GAS/SKCommonGameplayTagsLib.h"
#include "Logging/StructuredLog.h"

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
    Inventory = CreateDefaultSubobject<USKInventoryComponent>("Inventory component");

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
        AttributeSet = AbilitySystemComponent->GetSet<USKAttributeSet>();
        AttributeSetSkills = AbilitySystemComponent->GetSet<USKAttributeSetSkills>();
    }

    TryRunning();
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
        InteractibleActive = nullptr;
    }
}

//************************** GAS **************************
UAbilitySystemComponent *ASKBaseCharacter::GetAbilitySystemComponent() const
{
    return Cast<UAbilitySystemComponent>(AbilitySystemComponent.Get());
}

void ASKBaseCharacter::ActivateSprintAbility()
{

    if (AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_GrabbingItem())) return;

    const auto wantsToSpringTagContainer = USKCommonGameplayTagsLib::GetTag_WantsToSprint().GetSingleTagContainer();
    if (!AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_WantsToSprint()))
    {
        AbilitySystemComponent->AddLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_WantsToSprint());
        AbilitySystemComponent->TryActivateAbilitiesByTag(wantsToSpringTagContainer);

        if (bEnableLogging)
            UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' wants to sprint",
                      ("ActorName", this->GetName()));
    }
    else
    {

        AbilitySystemComponent->RemoveLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_WantsToSprint());
        AbilitySystemComponent->CancelAbilities(&wantsToSpringTagContainer);

        if (bEnableLogging)
            UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' no loger wants to sprint",
                      ("ActorName", this->GetName()));
    }
}

/************************************ MOVEMENT  ******************************************/

void ASKBaseCharacter::TrySrinting() const
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

        if (bEnableLogging)
            UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is now walking",
                      ("ActorName", this->GetName()));
    }
    else
    {
        AbilitySystemComponent->RemoveLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_Walking());
        TryRunning();
    }
}

float ASKBaseCharacter::GetCharacterMovementAngle()
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

    if (AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_Walking()))
    {
        AbilitySystemComponent->RemoveLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_Walking());
    }

    if (AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_Idling()))
    {
        AbilitySystemComponent->RemoveLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_Idling());
    }

    MovementComponent->StartRunning();
    OnStartedRunning.Broadcast();

    AbilitySystemComponent->AddLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_Running());

    if (bEnableLogging)
        UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is now running",
                  ("ActorName", this->GetName()));
}

/************************************ STATE  ******************************************/

void ASKBaseCharacter::HandleIdling()
{
    if (!bIsReceivingInput && GetVelocity().IsNearlyZero())
    {
        if (!AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_Idling()))
        {
            AbilitySystemComponent->AddLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_Idling());

            if (bEnableLogging)
                UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is now idling",
                          ("ActorName", this->GetName()));
        }
    }
    else
    {
        if (AbilitySystemComponent->HasMatchingGameplayTag(USKCommonGameplayTagsLib::GetTag_Idling()))
        {
            AbilitySystemComponent->RemoveLooseGameplayTag(USKCommonGameplayTagsLib::GetTag_Idling());

            if (bEnableLogging)
                UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' is no longer idling",
                          ("ActorName", this->GetName()));
        }
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
    if (!InteractibleActive.IsValid() || !AbilitySystemComponent->InteractAbility) return;

    const auto canInteractTag = FGameplayTag::RequestGameplayTag("Character.State.Action.CanInteract");
    const auto wantToInteractTag = FGameplayTag::RequestGameplayTag("Character.Request.Action.WantsToInteract");

    if (AbilitySystemComponent->HasMatchingGameplayTag(canInteractTag))
    {
        UE_LOGFMT(LogSKAbilitySystem, Display, "'{ActorName}' tried to interact with '{InteractableName}'!",
                  ("ActorName", this->GetName()), ("InteractableName", InteractibleActive->GetName()));

        if (AbilitySystemComponent->CheckAndAddGameplayTag(wantToInteractTag))
        {
            if (InteractibleActive->Implements<USKInterfaceCollectible>())
            {
                Inventory->AddToInventory(InteractibleActive.Get());
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