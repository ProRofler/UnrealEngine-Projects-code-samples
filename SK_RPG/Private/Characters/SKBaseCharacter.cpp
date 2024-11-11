// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/SKBaseCharacter.h"
#include "Async/Async.h"
#include "Characters/Components/SKCharacterMovementComponent.h"
#include "Characters/Components/SKInventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/Interface/SKInterfaceInteractable.h"
#include "Core/SKLogCategories.h"

ASKBaseCharacter::ASKBaseCharacter(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USKCharacterMovementComponent>(
          ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    InteractionZone = CreateDefaultSubobject<UCapsuleComponent>("Interactive zone");
    InteractionZone->SetCapsuleHalfHeight(130.0f);
    InteractionZone->SetCapsuleRadius(100.0f);
    InteractionZone->SetupAttachment(GetRootComponent());
    InteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    InteractionZone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    InteractionZone->SetGenerateOverlapEvents(true);
    InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &ASKBaseCharacter::OnBeginOverlap);
    InteractionZone->OnComponentEndOverlap.AddDynamic(this, &ASKBaseCharacter::OnOverlapEnd);

    MovementComponent = Cast<USKCharacterMovementComponent>(GetCharacterMovement());

    Inventory = CreateDefaultSubobject<USKInventoryComponent>("Character inventory");
}

void ASKBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(InteractionZone);
    check(MovementComponent);

    MovementType = EMovementType::ERunning;
}

void ASKBaseCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

//************************** MOVEMENT **************************
void ASKBaseCharacter::StartSprinting()
{
    if (GetActionType() == EActionType::EGrabbing || GetActionType() == EActionType::ERotating) return;

    bWalkToggle = false;
    bWantsToSprint = true;
    MovementType = EMovementType::ESprinting;
    MovementComponent->SetCharacterSpeed(MovementType);
}

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