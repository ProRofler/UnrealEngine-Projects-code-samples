// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKPhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/SKPlayerCharacter.h"

USKPhysicsHandleComponent::USKPhysicsHandleComponent() { Player = Cast<ASKPlayerCharacter>(GetOwner()); }

void USKPhysicsHandleComponent::GrabItem()
{
    if (!(Player->GetInteractibleActive().IsValid())) return;

    ItemToGrab = Player->GetInteractibleActive()->FindComponentByClass<UMeshComponent>();
    if (!ItemToGrab) return;

    FVector GrabPivot;
    FHitResult HitResult_pivot;

    if (Player->TraceFromCamera(HitResult_pivot, 250.0f) && HitResult_pivot.GetComponent() == ItemToGrab)
        GrabPivot = HitResult_pivot.ImpactPoint;
    else
        GrabPivot = ItemToGrab->GetComponentLocation();

    ItemToGrab->SetUseCCD(true);

    GrabComponentAtLocationWithRotation(ItemToGrab, NAME_None, GrabPivot, ItemToGrab->GetComponentRotation());

    InitialRelativeRotation = ItemToGrab->GetComponentRotation() - Player->GetActorRotation();

    Player->StartWalking();
    Player->SetActionType(EActionType::EGrabbing);
    Async(EAsyncExecution::ThreadIfForkSafe, [&]() { UpdateGrabLocation(); });
}

void USKPhysicsHandleComponent::UpdateGrabLocation()
{
    while (Player->GetActionType() == EActionType::EGrabbing || Player->GetActionType() == EActionType::ERotating)
    {
        if (IsValid(ItemToGrab) && GetWorld())
        {
            FVector GrabLocation;
            FHitResult HitResult_loc;

            if (Player->TraceFromCamera(HitResult_loc, Player->GrabDistance, ItemToGrab))
                GrabLocation = HitResult_loc.ImpactPoint;
            else
                GrabLocation = HitResult_loc.TraceEnd;
            SetTargetLocation(GrabLocation);

            if (!(Player->GetActionType() == EActionType::ERotating))
            {
                FRotator PlayerRotation = Player->GetActorRotation();
                FRotator NewRotation = PlayerRotation + InitialRelativeRotation;
                SetTargetRotation(NewRotation);
            }

            if (ItemToGrab && CheckDistanceToPlayer(ItemToGrab->GetOwner()) >= 40000.0f)
            {
                Async(EAsyncExecution::TaskGraphMainThread, [&]() { Player->HandleGrabbing(); });
                return;
            }
        }
        else
        {
            break;
        }
    }
}

void USKPhysicsHandleComponent::RotateGrabbedComponent(const FVector2D &Input)
{
    FVector HandleLoc;
    FRotator HandleRot;
    GetTargetLocationAndRotation(HandleLoc, HandleRot);

    HandleRot.Roll += Input.Y;
    HandleRot.Yaw += Input.X;
    HandleRot.Pitch = 0.0f;

    InitialRelativeRotation.Roll += Input.Y;
    InitialRelativeRotation.Yaw += Input.X;
    InitialRelativeRotation.Pitch = 0.0f;

    SetTargetRotation(HandleRot);
}

float USKPhysicsHandleComponent::CheckDistanceToPlayer(const AActor *OtherActor)
{
    const auto Distance =
        FVector::DistSquared(Player->PlayerCamera->GetComponentLocation(), OtherActor->GetActorLocation());

    return Distance;
}

void USKPhysicsHandleComponent::ReleaseItem()
{
    ReleaseComponent();
    GetItemToGrab()->SetUseCCD(false);
    ItemToGrab = nullptr;
    InitialRelativeRotation = FRotator::ZeroRotator;
    Player->SetActionType(EActionType::ENone);
    Player->StartRunning();
}
