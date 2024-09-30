// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/SKPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Characters/Components/SKInventoryComponent.h"
#include "Core/Interface/SKInterfaceInteractable.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

//********************* DEFAULT *********************
ASKPlayerCharacter::ASKPlayerCharacter(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    PlayerCamera = CreateDefaultSubobject<UCameraComponent>("Player camera");
    PlayerCamera->SetupAttachment(GetRootComponent());
    PlayerCamera->bUsePawnControlRotation = true;

    PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>("Physics handle");
}

void ASKPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

#if !UE_BUILD_SHIPPING
    PrintDebugInfo();
#endif
}

//********************* INPUT *********************
void ASKPlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    ControllerSetup();

    if (UEnhancedInputComponent *Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        Input->BindAction(InputData.MovingAction, ETriggerEvent::Triggered, this, &ASKPlayerCharacter::MoveAction);
        Input->BindAction(InputData.LookAction, ETriggerEvent::Triggered, this, &ASKPlayerCharacter::LookingAction);
        Input->BindAction(InputData.JumpAction, ETriggerEvent::Triggered, this, &ASKPlayerCharacter::Jump);
        Input->BindAction(InputData.SprintAction, ETriggerEvent::Triggered, this, &ASKPlayerCharacter::StartSprinting);
        Input->BindAction(InputData.SprintAction, ETriggerEvent::Completed, this, &ASKPlayerCharacter::StartRunning);
        Input->BindAction(InputData.WalkAction, ETriggerEvent::Triggered, this, &ASKPlayerCharacter::StartWalking);
        Input->BindAction(InputData.AltAction, ETriggerEvent::Triggered, this,
                          &ASKPlayerCharacter::HandleAlternativeAction);
        Input->BindAction(InputData.InteractionAction, ETriggerEvent::Triggered, this, &ASKPlayerCharacter::Interact);
        Input->BindAction(InputData.InteractionActionHold, ETriggerEvent::Triggered, this,
                          &ASKPlayerCharacter::HandleGrabbing);
    }
}

void ASKPlayerCharacter::ControllerSetup()
{
    if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(InputData.InputMapping, 0);
        }
    }
}

void ASKPlayerCharacter::MoveAction(const FInputActionValue &Value)
{
    const FVector2D MovingAxis = Value.Get<FVector2D>();

    AddMovementInput(GetActorForwardVector(), MovingAxis.X);
    AddMovementInput(GetActorRightVector(), MovingAxis.Y);
}

void ASKPlayerCharacter::LookingAction(const FInputActionValue &Value)
{
    const auto LookingAxisX = Value.Get<FVector2D>().X;
    const auto LookingAxisY = Value.Get<FVector2D>().Y * -1;

    if (GetActionType() != EActionType::ERotating)
    {
        AddControllerYawInput(LookingAxisX);
        AddControllerPitchInput(LookingAxisY);
    }
    else
    {
        RotateGrabbedComponent(Value.Get<FVector2D>());
    }
}

//********************* MULTITHREADING *********************
void ASKPlayerCharacter::GetLookedAtActor(TObjectPtr<AActor> &LookedAtActor) const
{
    double BestDotProduct = -1.0f;

    FRWScopeLock ReadLock(DataGuard, SLT_ReadOnly);

    for (const auto &Item : InteractablesInVicinity)
    {
        // get actor bounds
        FVector ActorBoundsOrigin, ActorBoxExtent;
        Item->GetActorBounds(false, ActorBoundsOrigin, ActorBoxExtent);

        // calcuate dot product
        const auto DotProduct = FVector::DotProduct(
            PlayerCamera->GetForwardVector(),
            UKismetMathLibrary::GetDirectionUnitVector(PlayerCamera->GetComponentLocation(), ActorBoundsOrigin));
        if (DotProduct >= BestDotProduct)
            BestDotProduct = DotProduct;
        else
            continue;

        // Minimally required dot product value to be considered
        const auto Threshold = FMath::Clamp(
            (FVector::Distance(PlayerCamera->GetComponentLocation(), ActorBoundsOrigin) / 10000.0f) + 0.95f, 0.0f,
            0.99f);

        if (BestDotProduct < Threshold)
            LookedAtActor = nullptr;
        else
            LookedAtActor = Item;
    }
}

void ASKPlayerCharacter::GrabItem()
{
    if (!InteractibleActive) return;

    GrabbedComponent = InteractibleActive->FindComponentByClass<UMeshComponent>();
    if (!GrabbedComponent)
    {
        return;
    }

    FVector GrabPivot;
    FHitResult HitResult_pivot;

    if (TraceFromCamera(HitResult_pivot, 250.0f) && HitResult_pivot.GetComponent() == GrabbedComponent)
        GrabPivot = HitResult_pivot.ImpactPoint;
    else
        GrabPivot = GrabbedComponent->GetComponentLocation();

    GrabbedComponent->SetUseCCD(true);

    PhysicsHandle->GrabComponentAtLocationWithRotation(GrabbedComponent, NAME_None, GrabPivot, FRotator::ZeroRotator);

    StartWalking();
    SetActionType(EActionType::EGrabbing);
    Async(EAsyncExecution::ThreadIfForkSafe, [&]() { UpdateGrabLocation(); });
}

void ASKPlayerCharacter::PrintDebugInfo() // DEBUG
{
    // showing the amount of items in vicinity
    if (DataGuard.TryReadLock())
    {

        GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue,
                                         "Items in list: " + FString::FromInt(InteractablesInVicinity.Num()), true);
        DataGuard.ReadUnlock();
    }

    // Show if can interact in the moment
    if (InteractibleActive)
        GEngine->AddOnScreenDebugMessage(2, 0.0f, FColor::Emerald, "I'm looking at: " + InteractibleActive->GetName(),
                                         true);

    // Current player state
    FString CurrentActionType = UEnum::GetValueAsString(GetActionType());
    FString CurrentMovementType = UEnum::GetValueAsString(GetMovementType());
    GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue,
                                     "Current states: " + CurrentMovementType + " | " + CurrentActionType, true);

    // Inventory
    if (Inventory)
    {
        GEngine->AddOnScreenDebugMessage(6, 0.0f, FColor::Cyan,
                                         "Items in inventory: " + FString::FromInt(Inventory->GetInventoryData().Num()),
                                         true);
    }
}

//********************* INTERACTIONS *********************

void ASKPlayerCharacter::HandleInteractionActor()
{
    GetLookedAtActor(InteractibleActive);
    if (!InteractibleActive) return;

    // final check with trace
    FHitResult TraceCheck = TraceToActor(InteractibleActive);

    if (!TraceCheck.bBlockingHit) return;

    // final comparison
    if (TraceCheck.GetActor() == InteractibleActive || TraceCheck.GetActor()->Implements<USKInterfaceInteractable>())

    {
        InteractibleActive = TraceCheck.GetActor();
    }

    else
    {
        InteractibleActive = nullptr;
    }
}

void ASKPlayerCharacter::Interact()
{
    if (GetActionType() == EActionType::EGrabbing)
    {
        ReleaseItem();
        SetActionType(EActionType::ENone);
    }
    else
    {
        ASKBaseCharacter::Interact();
    }
}

bool ASKPlayerCharacter::CanGrabItem()
{
    if (GetActionType() == EActionType::ENone && InteractibleActive)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ASKPlayerCharacter::UpdateGrabLocation()
{
    UE_LOG(LogTemp, Display, TEXT("Started updating grab loc"));

    while (GetActionType() == EActionType::EGrabbing || GetActionType() == EActionType::ERotating)
    {
        if (IsValid(GrabbedComponent) && IsValid(PhysicsHandle) && GetWorld())
        {
            FVector GrabLocation;
            FHitResult HitResult_loc;

            if (TraceFromCamera(HitResult_loc, GrabDistance, GrabbedComponent))
                GrabLocation = HitResult_loc.ImpactPoint;
            else
                GrabLocation = HitResult_loc.TraceEnd;
            PhysicsHandle->SetTargetLocation(GrabLocation);

            if (GrabbedComponent && CheckDistanceToPlayer(GrabbedComponent->GetOwner()) >= 40000.0f)
            {
                Async(EAsyncExecution::TaskGraphMainThread, [&]() { HandleGrabbing(); });
                return;
            }
        }
        else
        {
            break;
        }
    }
}

void ASKPlayerCharacter::HandleGrabbing()
{
    if (GetActionType() == EActionType::EGrabbing || GetActionType() == EActionType::ERotating)
    {
        ReleaseItem();
        return;
    }
    else if (!CanGrabItem())
    {
        return;
    }
    else
    {
        SetActionType(EActionType::EGrabbing);
        GrabItem();
    }
}

void ASKPlayerCharacter::ReleaseItem()
{
    PhysicsHandle->ReleaseComponent();
    GrabbedComponent->SetUseCCD(false);
    GrabbedComponent = nullptr;
    SetActionType(EActionType::ENone);
    StartRunning();
}

void ASKPlayerCharacter::RotateGrabbedComponent(const FVector2D &Input) const
{
    FVector HandleLoc;
    FRotator HandleRot;
    PhysicsHandle->GetTargetLocationAndRotation(HandleLoc, HandleRot);

    HandleRot.Roll += Input.Y;
    HandleRot.Yaw += Input.X;

    PhysicsHandle->SetTargetRotation(HandleRot);
}

float ASKPlayerCharacter::CheckDistanceToPlayer(const TObjectPtr<AActor> OtherActor)
{
    const auto Distance = FVector::DistSquared(PlayerCamera->GetComponentLocation(), OtherActor->GetActorLocation());

    return Distance;
}

void ASKPlayerCharacter::HandleAlternativeAction()
{
    if (GetActionType() == EActionType::EGrabbing)
    {
        SetActionType(EActionType::ERotating);
    }
    else if (GetActionType() == EActionType::ERotating)
    {
        SetActionType(EActionType::EGrabbing);
    }
    else
    {
        return;
    }
}

//********************* UTILS *********************
FHitResult ASKPlayerCharacter::TraceToActor(const TObjectPtr<AActor> &OtherActor) const
{

    FHitResult HitResult;

    GetWorld()->LineTraceSingleByChannel(HitResult, PlayerCamera->GetComponentLocation(),
                                         OtherActor->GetActorLocation(), ECollisionChannel::ECC_Visibility);

    return HitResult;
}

bool ASKPlayerCharacter::TraceFromCamera(FHitResult &HitResult, const float TraceDistance)
{
    FVector TraceStart = PlayerCamera->GetComponentLocation();
    FVector TracecEnd = TraceStart + (PlayerCamera->GetForwardVector() * TraceDistance);

    return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TracecEnd, ECollisionChannel::ECC_Visibility);
}

bool ASKPlayerCharacter::TraceFromCamera(FHitResult &HitResult, const float TraceDistance,
                                         const TObjectPtr<UMeshComponent> ComponentToIgnore)
{
    FVector TraceStart = PlayerCamera->GetComponentLocation();
    FVector TracecEnd = TraceStart + (PlayerCamera->GetForwardVector() * TraceDistance);

    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredComponent(ComponentToIgnore);

    return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TracecEnd, ECollisionChannel::ECC_Visibility,
                                                TraceParams);
}
