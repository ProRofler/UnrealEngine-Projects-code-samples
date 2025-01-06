// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/SKPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/Components/SKPhysicsHandleComponent.h"
#include "Controllers/SKPlayerController.h"
#include "Core/Interface/SKInterfaceInteractable.h"
#include "Core/SKCoreTypes.h"
#include "Core/SKLogCategories.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/SKPlayerHUD.h"
#include "UI/Widgets/SKInventoryWidget.h"

static const auto canInteractTagB = FGameplayTag::RequestGameplayTag("Character.State.Action.CanInteract");
static const auto grabbingTagB = FGameplayTag::RequestGameplayTag("Character.State.Action.Grabbing");
static const auto rotatingTagB = FGameplayTag::RequestGameplayTag("Character.State.Action.Rotating");

/********************* DEFAULT *********************/
ASKPlayerCharacter::ASKPlayerCharacter(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    PlayerCamera = CreateDefaultSubobject<UCameraComponent>("Player camera");
    PlayerCamera->SetupAttachment(GetRootComponent());
    PlayerCamera->bUsePawnControlRotation = true;

    PhysicsHandle = CreateDefaultSubobject<USKPhysicsHandleComponent>("Physics handle");
}

void ASKPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

#if !UE_BUILD_SHIPPING
    PrintDebugInfo();
#endif
}

void ASKPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    InitializeComponents();
}

/********************* INPUT *********************/
void ASKPlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    ControllerSetup();

    if (UEnhancedInputComponent *Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {

        Input->BindAction(InputData.MovingAction, ETriggerEvent::Triggered, this, &ASKPlayerCharacter::MoveAction);
        Input->BindAction(InputData.MovingAction, ETriggerEvent::Completed, this, &ASKPlayerCharacter::MoveAction);
        Input->BindAction(InputData.LookAction, ETriggerEvent::Triggered, this, &ASKPlayerCharacter::LookingAction);
        Input->BindAction(InputData.LookAction, ETriggerEvent::Completed, this, &ASKPlayerCharacter::LookingAction);
        Input->BindAction(InputData.JumpAction, ETriggerEvent::Triggered, this, &ASKPlayerCharacter::Jump);
        Input->BindAction(InputData.SprintAction, ETriggerEvent::Triggered, this,
                          &ASKPlayerCharacter::ActivateSprintAbility);
        Input->BindAction(InputData.WalkAction, ETriggerEvent::Triggered, this, &ASKPlayerCharacter::TryWalking);
        Input->BindAction(InputData.AltAction, ETriggerEvent::Triggered, this,
                          &ASKPlayerCharacter::HandleAlternativeAction);
        Input->BindAction(InputData.InteractionAction, ETriggerEvent::Triggered, this, &ASKPlayerCharacter::Interact);
        Input->BindAction(InputData.InteractionActionHold, ETriggerEvent::Triggered, this,
                          &ASKPlayerCharacter::HandleGrabbing);
        Input->BindAction(InputData.InventoryToggleAction, ETriggerEvent::Triggered, this,
                          &ASKPlayerCharacter::HandleInventoryToggle);
    }
}

void ASKPlayerCharacter::ControllerSetup()
{
    if (APlayerController *PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(InputData.InputMapping, 0);
        }
    }
}

void ASKPlayerCharacter::MoveAction(const FInputActionValue &Value)
{
    if (Value.IsNonZero())
    {
        bIsReceivingInput = true;
        // UE_LOG(LogSKCharacterMovement, Display, TEXT("Player is receiving movement input"));

        const FVector2D MovingAxis = Value.Get<FVector2D>();
        AddMovementInput(GetActorForwardVector(), MovingAxis.X);
        AddMovementInput(GetActorRightVector(), MovingAxis.Y);
    }
    else
    {
        // UE_LOG(LogSKCharacterMovement, Display, TEXT("Player is no longer receiving movement input"));

        bIsReceivingInput = false;
    }
}

void ASKPlayerCharacter::LookingAction(const FInputActionValue &Value)
{
    if (Value.IsNonZero())
    {
        bIsReceivingInput = true;
        // UE_LOG(LogSKCharacterMovement, Display, TEXT("Player receiving looking input"));

        const auto LookingAxisX = Value.Get<FVector2D>().X;
        const auto LookingAxisY = Value.Get<FVector2D>().Y * -1;

        if (!AbilitySystemComponent->HasMatchingGameplayTag(rotatingTagB))
        {
            AddControllerYawInput(LookingAxisX);
            AddControllerPitchInput(LookingAxisY);
        }
        else
        {
            PhysicsHandle->RotateGrabbedComponent(Value.Get<FVector2D>());
        }
    }
    else
    {
        bIsReceivingInput = false;
        // UE_LOG(LogSKCharacterMovement, Display, TEXT("Player is no longer receiving looking input"));
    }
}

/********************* MULTITHREADING *********************/
AActor *ASKPlayerCharacter::GetLookedAtActor() const
{
    double BestDotProduct = -1.0f;
    double Threshold;
    AActor *Item = nullptr;

    FRWScopeLock ReadLock(DataGuard, SLT_ReadOnly);

    for (const auto &ItemInVicinity : InteractablesInVicinity)
    {
        // get actor bounds
        FVector ActorBoundsOrigin, ActorBoxExtent;
        ItemInVicinity->GetActorBounds(false, ActorBoundsOrigin, ActorBoxExtent);

        // calcuate dot product
        const auto DotProduct = FVector::DotProduct(
            PlayerCamera->GetForwardVector(),
            UKismetMathLibrary::GetDirectionUnitVector(PlayerCamera->GetComponentLocation(), ActorBoundsOrigin));
        if (DotProduct >= BestDotProduct)
            BestDotProduct = DotProduct;
        else
            continue;

        // Minimally required dot product value to be considered
        Threshold = FMath::Clamp(
            (FVector::Distance(PlayerCamera->GetComponentLocation(), ActorBoundsOrigin) / 10000.0f) + 0.95f, 0.0f,
            0.99f);

        Item = ItemInVicinity;
    }

    if (BestDotProduct < Threshold)
        return nullptr;
    else
        return Item;
}

void ASKPlayerCharacter::PrintDebugInfo()
{
    // Vicinity Debug
    /*
        // showing the amount of items in vicinity
        if (DataGuard.TryReadLock())
        {

            GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue,
                                             "Items in list: " + FString::FromInt(InteractablesInVicinity.Num()), true);
            DataGuard.ReadUnlock();
        }

        // Show if can interact in the moment
        if (InteractibleActive.IsValid())
        {
            GEngine->AddOnScreenDebugMessage(2, 0.0f, FColor::Emerald, "I'm looking at: " +
       InteractibleActive->GetName(), true);
        }
        */
    // Draw XY arrows for physics handle debug
    /*
    if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
    {
        // �������� ������� ��������� � ������� Physics Handle
        FVector HandleLocation;
        FRotator HandleRotation;
        PhysicsHandle->GetTargetLocationAndRotation(HandleLocation, HandleRotation);

        // ����� �������
        float ArrowLength = 30.0f;

        // ������� �� ��� X (�������)
        FVector XDirection = HandleRotation.RotateVector(FVector::ForwardVector) * ArrowLength;
        FVector XArrowEnd = HandleLocation + XDirection;
        DrawDebugDirectionalArrow(GetWorld(), HandleLocation, XArrowEnd,
                                  25.0f,       // ����� "�������" ������
                                  FColor::Red, // ���� ��� ��� X
                                  false,       // ���������� ���������
                                  -1.0f,       // ����� ����� ������
                                  0,           // ���������
                                  2.0f         // ������� �������
        );

        // ������� �� ��� Z (�����)
        FVector ZDirection = HandleRotation.RotateVector(FVector::UpVector) * ArrowLength;
        FVector ZArrowEnd = HandleLocation + ZDirection;
        DrawDebugDirectionalArrow(GetWorld(), HandleLocation, ZArrowEnd,
                                  25.0f,        // ����� "�������" ������
                                  FColor::Blue, // ���� ��� ��� Z
                                  false,        // ���������� ���������
                                  -1.0f,        // ����� ����� ������
                                  0,            // ���������
                                  2.0f          // ������� �������
        );
    } */

    // Interactible active rotation debug info
    /*
    if (InteractibleActive.IsValid())
    {
        // �������� ������� ��������� �������
        FRotator InteractibleRotation = InteractibleActive->GetActorRotation();

        // ������� ������� ��������� ������� �� �����
        GEngine->AddOnScreenDebugMessage(
            -1, // ���������� ID ���������, -1 ��������, ��� ��������� �������� ����� �����
            0,  // ����� ����������� ���������
            FColor::Cyan, // ���� ������
            FString::Printf(TEXT("InteractibleActive Rotation: Pitch: %f, Yaw: %f, Roll: %f"),
                            InteractibleRotation.Pitch, InteractibleRotation.Yaw, InteractibleRotation.Roll));
    }*/

    // Phys handle rotation debug info
    /*
    if (PhysicsHandle)
    {
        // �������� TargetRotation Physics Handle
        FRotator PhysicsHandleTargetRotation;
        FVector t;
        PhysicsHandle->GetTargetLocationAndRotation(t, PhysicsHandleTargetRotation);

        // ������� TargetRotation Physics Handle �� �����
        GEngine->AddOnScreenDebugMessage(
            -1, // ���������� ID ���������, -1 ��������, ��� ��������� �������� ����� �����
            0,  // ����� ����������� ���������
            FColor::Green, // ���� ������
            FString::Printf(TEXT("PhysicsHandle Target Rotation: Pitch: %f, Yaw: %f, Roll: %f"),
                            PhysicsHandleTargetRotation.Pitch, PhysicsHandleTargetRotation.Yaw,
                            PhysicsHandleTargetRotation.Roll));
    }*/

    // Inventory debug info
    /*
    if (Inventory)
    {
        GEngine->AddOnScreenDebugMessage(6, 0.0f, FColor::Cyan,
                                         "Items in inventory: " + FString::FromInt(Inventory->GetInventoryData().Num()),
                                         true);
    } */
}

/********************* INTERACTIONS *********************/

void ASKPlayerCharacter::HandleInteractionActor()
{
    InteractibleActive = GetLookedAtActor();

    if (!InteractibleActive.IsValid())
    {
        AbilitySystemComponent->CheckAndRemoveGameplayTag(canInteractTagB);
        return;
    }

    // final check with trace
    FHitResult TraceCheck = TraceToActor(InteractibleActive.Get());

    if (!TraceCheck.bBlockingHit) return;

    // final comparison
    if (TraceCheck.GetActor() == InteractibleActive || TraceCheck.GetActor()->Implements<USKInterfaceInteractable>())

    {
        InteractibleActive = TraceCheck.GetActor();
        AbilitySystemComponent->CheckAndAddGameplayTag(canInteractTagB);
    }

    else
    {
        InteractibleActive = nullptr;
    }
}

void ASKPlayerCharacter::Interact()
{
    if (AbilitySystemComponent->HasMatchingGameplayTag(grabbingTagB))
    {
        PhysicsHandle->ReleaseItem();
        AbilitySystemComponent->RemoveLooseGameplayTag(grabbingTagB);
        AbilitySystemComponent->RemoveLooseGameplayTag(rotatingTagB);
    }
    else if (!InteractibleActive.IsValid())
    {
        return;
    }
    else
    {
        ASKBaseCharacter::Interact();
        if (PlayerController->GetPlayerHUD()->IsInventoryOpen())
            PlayerController->GetPlayerHUD()->GetInventoryWidget()->UpdateInventoryWidget();
    }
}

void ASKPlayerCharacter::DropItem(AActor *ItemToDrop)
{
    if (!ItemToDrop) return;

    // TODO: Drop item logic
    Inventory->RemoveFromInventory(ItemToDrop);

    FHitResult dropPosition;

    if (TraceFromCamera(dropPosition, 150.0f))
    {
        ItemToDrop->SetActorLocation(dropPosition.ImpactPoint);
    }
    else
    {
        ItemToDrop->SetActorLocation(dropPosition.TraceEnd);
    }
}

bool ASKPlayerCharacter::CanGrabItem()
{
    if (!InteractibleActive.IsValid()) return false;

    if (AbilitySystemComponent->HasMatchingGameplayTag(canInteractTagB) &&
        !AbilitySystemComponent->HasMatchingGameplayTag(grabbingTagB) &&
        InteractibleActive->GetRootComponent()->IsSimulatingPhysics()) // TODO: Check by interface in the future
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ASKPlayerCharacter::HandleGrabbing()
{
    if (CanGrabItem())
    {
        AbilitySystemComponent->CheckAndAddGameplayTag(grabbingTagB);
        PhysicsHandle->GrabItem();
    }
    else if (!CanGrabItem() && PhysicsHandle->GetItemToGrab())
    {
        AbilitySystemComponent->CheckAndRemoveGameplayTag(grabbingTagB);
        AbilitySystemComponent->CheckAndRemoveGameplayTag(rotatingTagB);
        PhysicsHandle->ReleaseItem();
    }
}

void ASKPlayerCharacter::HandleAlternativeAction()
{
    if (AbilitySystemComponent->HasMatchingGameplayTag(grabbingTagB) &&
        !AbilitySystemComponent->HasMatchingGameplayTag(rotatingTagB))
    {
        AbilitySystemComponent->AddLooseGameplayTag(rotatingTagB);
    }
    else if (AbilitySystemComponent->HasMatchingGameplayTag(rotatingTagB))
    {
        AbilitySystemComponent->RemoveLooseGameplayTag(rotatingTagB);
    }
    else
    {
        return;
    }
}

/*********************** UI ***********************/
void ASKPlayerCharacter::HandleInventoryToggle() { PlayerController->ToggleInventoryHUD(); }

/********************* UTILS *********************/

void ASKPlayerCharacter::InitializeComponents()
{
    PlayerController = Cast<ASKPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    check(PlayerController.Get());
}

FHitResult ASKPlayerCharacter::TraceToActor(const AActor *OtherActor) const
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
                                         const UMeshComponent *ComponentToIgnore)
{
    FVector TraceStart = PlayerCamera->GetComponentLocation();
    FVector TracecEnd = TraceStart + (PlayerCamera->GetForwardVector() * TraceDistance);

    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredComponent(ComponentToIgnore);

    return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TracecEnd, ECollisionChannel::ECC_Visibility,
                                                TraceParams);
}