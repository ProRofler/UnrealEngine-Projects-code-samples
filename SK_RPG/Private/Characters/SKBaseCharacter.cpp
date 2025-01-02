// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/SKBaseCharacter.h"
#include "Async/Async.h"
#include "Characters/Components/SKCharacterMovementComponent.h"
#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/Components/SKStateMachineComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/Interface/SKInterfaceInteractable.h"
#include "Core/SKLogCategories.h"
#include "Gameplay/GAS/Abilities/SKAbilityBase.h"
#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Gameplay/GAS/SKAttributeSet.h"
#include "Gameplay/GAS/SKAttributeSetSkills.h"
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
    // check(AttributeSet);

    if (IsValid(AbilitySystemComponent))
    {
        AttributeSet = AbilitySystemComponent->GetSet<USKAttributeSet>();
        AttributeSetSkills = AbilitySystemComponent->GetSet<USKAttributeSetSkills>();
    }

    MovementType = EMovementType::ERunning;
}

void ASKBaseCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

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
    /*
    if (GetActionType() == EActionType::EGrabbing || GetActionType() == EActionType::ERotating) return;

    bWalkToggle = false;
    bWantsToSprint = true;
    MovementType = EMovementType::ESprinting;
    MovementComponent->SetCharacterSpeed(MovementType);
    */

    auto wantToSprintTag = FGameplayTag::RequestGameplayTag("Character.Request.Movement.WantsToSprint");
    const auto tagContainer = wantToSprintTag.GetSingleTagContainer();

    if (!AbilitySystemComponent->HasMatchingGameplayTag(wantToSprintTag))
    {
        AbilitySystemComponent->AddLooseGameplayTag(wantToSprintTag);
        AbilitySystemComponent->TryActivateAbilitiesByTag(tagContainer);

        UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' wants to sprint",
                  ("ActorName", this->GetName()));
    }
    else
    {

        AbilitySystemComponent->RemoveLooseGameplayTag(wantToSprintTag);
        AbilitySystemComponent->CancelAbilities(&tagContainer);

        UE_LOGFMT(LogSKCharacterMovement, Display, "Actor '{ActorName}' no loger wants to sprint",
                  ("ActorName", this->GetName()));
    }
}

/************************************ MOVEMENT  ******************************************/

void ASKBaseCharacter::StartSrinting() const { MovementComponent->StartSprinting(); }

void ASKBaseCharacter::StartRunning() const { MovementComponent->StartRunning(); }

// OLD
void ASKBaseCharacter::StartWalking()
{
    if (bWantsToSprint || GetActionType() == EActionType::ERotating || GetActionType() == EActionType::EGrabbing)
        return;

    if (bWalkToggle)
    {
        bWalkToggle = false;
        StartRunning();
        return;
    }

    bWalkToggle = true;
    MovementType = EMovementType::EWalking;
    MovementComponent->SetCharacterSpeed(MovementType);
}

void ASKBaseCharacter::StartRunning()
{
    bWalkToggle = false;
    bWantsToSprint = false;

    MovementType = EMovementType::ERunning;
    MovementComponent->SetCharacterSpeed(MovementType);
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

void ASKBaseCharacter::TakeItem()
{
    if (!InteractibleActive.IsValid()) return;

    Inventory->AddToInventory(InteractibleActive.Get());
}