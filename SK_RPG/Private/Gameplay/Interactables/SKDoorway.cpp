// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKDoorway.h"

#include "Curves/CurveFloat.h"

#include "Gameplay/Interactables/Components/SKLockComponent.h"
#include "Gameplay/Interactables/SKActivator.h"

#include "Gameplay/Interactables/SKKeyItem.h"

#include "Core/Interface/SKInterfaceCharacter.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

ASKDoorway::ASKDoorway()
{
    PrimaryActorTick.bCanEverTick = true;
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Door mesh");
    DoorMesh->SetupAttachment(GetRootComponent());

    OpenedDoorHandleComponent = CreateDefaultSubobject<USceneComponent>("Opened door handle");
    OpenedDoorHandleComponent->SetupAttachment(GetRootComponent());

    DoorGhostMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ghost door mesh");
    DoorGhostMesh->SetupAttachment(OpenedDoorHandleComponent);

    OpenedDoorHandleComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

    SetupGhostMaterial();
}

void ASKDoorway::BeginPlay()
{
    Super::BeginPlay();

    if (!AnimationCurve)
    {
        UE_LOGFMT(LogSKInteractions, Error, "{this} has no opening animation!", ("this", this->GetName()));
        checkNoEntry();
    }

    FOnTimelineFloat TimelineCallback;
    TimelineCallback.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(ASKDoorway, HandleDoorAnimation));
    if (!TimelineCallback.IsBound()) checkNoEntry();

    DoorTimeline.AddInterpFloat(AnimationCurve, TimelineCallback);

    StartTransform = DoorMesh->GetRelativeTransform();
    EndTransform = OpenedDoorHandleComponent->GetRelativeTransform();

    DoorGhostMesh->DestroyComponent();
    OpenedDoorHandleComponent->DestroyComponent();
}

void ASKDoorway::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    DoorTimeline.TickTimeline(DeltaSeconds);
}

void ASKDoorway::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);

    if (DoorMesh->GetStaticMesh())
    {
        DoorGhostMesh->SetStaticMesh(DoorMesh->GetStaticMesh());
        if (bShowGhostDoor)
        {
            DoorGhostMesh->SetVisibility(true);
        }
        else
        {
            DoorGhostMesh->SetVisibility(false);
        }

        if (GhostMat) AssignGhostMaterial();
    }

    if (bRemoteActivationOnly)
    {
        bHasLock = false;
        if (LockComponent) LockComponent->DestroyComponent();
    }

    if (bHasLock)
    {
        if (!LockComponent)
        {
            LockComponent = NewObject<USKLockComponent>(this);
            if (LockComponent)
            {
                LockComponent->RegisterComponent();
                LockComponent->SetFlags(RF_Transactional); // Do I need this?
            }
        }
    }
    else
    {
        if (LockComponent)
        {
            LockComponent->DestroyComponent();
            LockComponent = nullptr;
        }
    }
}

void ASKDoorway::OnInteraction_Implementation(const AActor *TriggeredActor)
{
    if (bRemoteActivationOnly && !Cast<ASKActivator>(TriggeredActor)) return;

    if (TriggeredActor->Implements<USKInterfaceCharacter>())

        if (bHasLock && LockComponent)
        {
            if (LockComponent->IsLocked())
            {
                if (!LockComponent->TryUnlocking(TriggeredActor)) return;
            }
        }

    if (bHasLock && LockComponent)
    {
        if (!LockComponent->IsLocked()) HandleDoorOpenClose();
    }
    else
    {
        HandleDoorOpenClose();
    }

    if (bEnableLogging)
    {
        UE_LOGFMT(LogSKInteractions, Display, "{Actor} tried to activate {this}", ("Actor", TriggeredActor->GetName()),
                  ("this", this->GetName()));
    }
}

void ASKDoorway::HandleDoorAnimation(float Value)
{
    FTransform blendVal;
    blendVal.Blend(StartTransform, EndTransform, Value);

    DoorMesh->SetRelativeTransform(blendVal);

    if (FMath::IsNearlyEqual(Value, 1.0f) || FMath::IsNearlyEqual(Value, 0.0f)) bIsActive = false;
}

void ASKDoorway::SetupGhostMaterial()
{

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(
        TEXT("/Engine/VREditor/UI/FrameMaterial.FrameMaterial"));
    if (MaterialFinder.Succeeded())
    {
        GhostMat = MaterialFinder.Object;
    }
}

void ASKDoorway::AssignGhostMaterial()
{
    for (int i = 0; i < DoorGhostMesh->GetNumMaterials(); i++)
    {
        DoorGhostMesh->SetMaterial(i, GhostMat);
    }
}

void ASKDoorway::HandleDoorOpenClose()
{
    if (!bIsOpened)
    {
        DoorTimeline.Play();
        bIsOpened = !bIsOpened;
        bIsActive = true;
    }
    else
    {
        DoorTimeline.Reverse();
        bIsOpened = !bIsOpened;
        bIsActive = true;
    }
}
