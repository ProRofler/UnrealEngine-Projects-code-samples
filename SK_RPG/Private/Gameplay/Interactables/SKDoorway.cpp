// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKDoorway.h"
#include "Core/SKLogCategories.h"
#include "Curves/CurveFloat.h"
#include "Logging/StructuredLog.h"

ASKDoorway::ASKDoorway()
{
    PrimaryActorTick.bCanEverTick = true;
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Door mesh");
    DoorMesh->SetupAttachment(GetRootComponent());
}

void ASKDoorway::BeginPlay()
{
    Super::BeginPlay();

    if (AnimationCurve)
    {
    }
    else
    {
        UE_LOGFMT(LogSKInteractions, Error, "{this} has no opening animation!", ("this", this->GetName()));
        checkNoEntry();
    }

    FOnTimelineFloat TimelineCallback;
    TimelineCallback.BindUFunction(this, FName("HandleDoorOpenClose"));
    if (!TimelineCallback.IsBound()) checkNoEntry();

    DoorTimeline.AddInterpFloat(AnimationCurve, TimelineCallback);

    endRot = startRot = DoorMesh->GetRelativeRotation();
    endRot.Yaw += OpeningAngle;
}

void ASKDoorway::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    DoorTimeline.TickTimeline(DeltaSeconds);
}

void ASKDoorway::OnInteraction_Implementation(const AActor *TriggeredActor)
{

    if (!bIsOpened)
    {
        DoorTimeline.Play();
        bIsOpened = !bIsOpened;
    }
    else
    {
        DoorTimeline.Reverse();
        bIsOpened = !bIsOpened;
    }

    if (bEnableLogging)
    {
        UE_LOGFMT(LogSKInteractions, Display, "{Actor} tried to activate {this}", ("Actor", TriggeredActor->GetName()),
                  ("this", this->GetName()));
    }
}

void ASKDoorway::HandleDoorOpenClose(float Value)
{
    const auto lerpVal = FMath::Lerp(startRot, endRot, Value);

    DoorMesh->SetRelativeRotation(lerpVal);

    UE_LOG(LogTemp, Warning, TEXT("Timeline Value: %f, lerp value: %f"), Value, lerpVal.Yaw);
}
