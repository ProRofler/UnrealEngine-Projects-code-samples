// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKPhysicsHandleComponent.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/SKPlayerCharacter.h"
#include "Gameplay/GAS/SKNativeGameplayTags.h"

void USKPhysicsHandleComponent::BeginPlay()
{
    Super::BeginPlay();

    Player = CastChecked<ASKPlayerCharacter>(GetOwner());
}

void USKPhysicsHandleComponent::GrabItem(UPrimitiveComponent *ComponentToGrab)
{
    if (!ComponentToGrab) return;

    FVector GrabPivot;
    FHitResult HitResult_pivot;

    if (Player->TraceFromCamera(HitResult_pivot, 250.0f) && HitResult_pivot.GetComponent() == ComponentToGrab)
        GrabPivot = HitResult_pivot.ImpactPoint;
    else
        GrabPivot = ComponentToGrab->GetComponentLocation();

    ComponentToGrab->SetUseCCD(true);

    GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, GrabPivot, ComponentToGrab->GetComponentRotation());

    InitialRelativeRotation = ComponentToGrab->GetComponentRotation() - Player->GetActorRotation();

    Player->TryWalking();
    Async(EAsyncExecution::ThreadIfForkSafe, [&]() { UpdateGrabLocation(); });
}

void USKPhysicsHandleComponent::UpdateGrabLocation()
{
    const auto grabbingTag = FSKGameplayTags::Get().Character_State_Action_GrabbingItem;
    const auto rotatingTag = FSKGameplayTags::Get().Character_State_Action_RotatingItem;

    while (Player->GetAbilitySystemComponent()->HasMatchingGameplayTag(grabbingTag))
    {
        if (GrabbedComponent && GetWorld())
        {
            FVector GrabLocation;
            FHitResult HitResult_loc;

            if (Player->TraceFromCamera(HitResult_loc, Player->GrabDistance, GrabbedComponent))
                GrabLocation = HitResult_loc.ImpactPoint;
            else
                GrabLocation = HitResult_loc.TraceEnd;
            SetTargetLocation(GrabLocation);

            if (!(Player->GetAbilitySystemComponent()->HasMatchingGameplayTag(rotatingTag)))
            {
                FRotator PlayerRotation = Player->GetActorRotation();
                FRotator NewRotation = PlayerRotation + InitialRelativeRotation;
                SetTargetRotation(NewRotation);
            }

            if (GrabbedComponent && CheckDistanceToPlayer(GrabbedComponent->GetOwner()) >= 40000.0f)
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
    if (!GrabbedComponent) return;

    FVector HandleLoc;
    FRotator HandleRot;
    GetTargetLocationAndRotation(HandleLoc, HandleRot);

    HandleRot.Roll = 0.0f;
    HandleRot.Yaw += Input.X;
    HandleRot.Pitch += Input.Y;

    InitialRelativeRotation.Roll = 0.0f;
    InitialRelativeRotation.Yaw += Input.X;
    InitialRelativeRotation.Pitch += Input.Y;

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
    GrabbedComponent->SetUseCCD(false);
    ReleaseComponent();
    InitialRelativeRotation = FRotator::ZeroRotator;
    Player->TryRunning();
}
