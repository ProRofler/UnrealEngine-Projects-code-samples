// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/SKPlayerCharacter.h"

#include "Camera/CameraComponent.h"

#include "Characters/Components/SKInteractionComponent.h"
#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/Components/SKPhysicsHandleComponent.h"

#include "Core/Interface/SKInterfaceInteractable.h"
#include "Core/SKCoreTypes.h"
#include "Gameplay/Interactables/SKKeyItem.h"

#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Gameplay/GAS/SKNativeGameplayTags.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "UI/Data/SKInventoryObjectData.h"
#include "UI/SKPlayerHUD.h"
#include "UI/Widgets/SKInventoryWidget.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

#include "Controllers/SKPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "ProfilingDebugging/CpuProfilerTrace.h"

ASKPlayerCharacter::ASKPlayerCharacter(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    PlayerCamera = CreateDefaultSubobject<UCameraComponent>("Player camera");
    PlayerCamera->SetupAttachment(GetRootComponent());
    PlayerCamera->bUsePawnControlRotation = true;

    // GetMesh()->SetupAttachment(PlayerCamera);

    PhysicsHandle = CreateDefaultSubobject<USKPhysicsHandleComponent>("Physics handle");
}
/************************************ UE INHERITED ******************************************/

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
    SKPlayerController = CastChecked<ASKPlayerController>(GetController());
}

/********************* INPUT *********************/

void ASKPlayerCharacter::HandleDeath()
{
    Super::HandleDeath();

    if (PlayerCamera && GetMesh())
    {
        GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

        PlayerCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                                        TEXT("headSocket"));
    }
    DisableInput(SKPlayerController.Get());
    SKPlayerController->SetIgnoreLookInput(true);
}

void ASKPlayerCharacter::MoveAction(const FInputActionValue &Value)
{
    if (Value.IsNonZero())
    {
        if (bEnableLogging && bEnableLoggingInput)
            UE_LOG(LogSKCharacterMovement, Display, TEXT("Player is receiving movement input"));

        const FVector2D MovingAxis = Value.Get<FVector2D>();
        AddMovementInput(GetActorForwardVector(), MovingAxis.X);
        AddMovementInput(GetActorRightVector(), MovingAxis.Y);
    }
}

void ASKPlayerCharacter::LookingAction(const FInputActionValue &Value)
{
    if (Value.IsNonZero())
    {
        if (bEnableLogging && bEnableLoggingInput)
            UE_LOG(LogSKCharacterMovement, Display, TEXT("Player receiving looking input"));

        if (Value.GetMagnitude() > 0.2f)
        {
            bIsLookIdle = true;
        }

        const auto LookingAxisX = Value.Get<FVector2D>().X;
        const auto LookingAxisY = Value.Get<FVector2D>().Y * -1;

        if (!AbilitySystemComponent->HasMatchingGameplayTag(FSKGameplayTags::Get().Character_State_Action_RotatingItem))
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
        bIsLookIdle = false;
    }
}
/************************************ State  ******************************************/

void ASKPlayerCharacter::HandleIdling()
{
    if (!IsCharacterMoving() && !bIsLookIdle) // TODO: add mouse input check
    {
        StartIdle();
    }
    else
    {
        StopIdle();
    }
}

/************************************ DEBUG ******************************************/

void ASKPlayerCharacter::PrintDebugInfo()
{
    // Vicinity Debug

    // showing the amount of items in vicinity
    /*
     if (DataGuard.TryReadLock())
    {

        GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue,
                                         "Items in list: " + FString::FromInt(InteractablesInVicinity.Num()), true);
        DataGuard.ReadUnlock();
    } */

    // Show if can interact in the moment
    /* if (InteractionTarget.IsValid())
    {
        GEngine->AddOnScreenDebugMessage(2, 0.0f, FColor::Emerald, "I'm looking at: " + InteractionTarget->GetName(),
                                         true);
    }*/

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
    if (InteractionTarget.IsValid())
    {
        // �������� ������� ��������� �������
        FRotator InteractibleRotation = InteractionTarget->GetActorRotation();

        // ������� ������� ��������� ������� �� �����
        GEngine->AddOnScreenDebugMessage(
            -1, // ���������� ID ���������, -1 ��������, ��� ��������� �������� ����� �����
            0,  // ����� ����������� ���������
            FColor::Cyan, // ���� ������
            FString::Printf(TEXT("InteractionTarget Rotation: Pitch: %f, Yaw: %f, Roll: %f"),
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

        FString items;

        for (auto data : GetInventoryComponent()->GetInventoryData())
        {
            items.Append(data->GetItemName().ToString() + "\n");
        }

        GEngine->AddOnScreenDebugMessage(7, 0.0f, FColor::Yellow, "Items: \n" + items, true);
    }
    */
}

/********************* INTERACTIONS *********************/

void ASKPlayerCharacter::DropItem(USKInventoryObjectData *ItemToRemove, const int32 QuantityToDrop)
{
    if (!ItemToRemove) return;

    if (!InventoryComponent->RemoveFromInventory(ItemToRemove, QuantityToDrop)) return;

    FHitResult dropPosition;

    if (TraceFromCamera(dropPosition, 150.0f))
    {
        AActor *SpawnedItem = GetWorld()->SpawnActor<AActor>(
            ItemToRemove->GetItemClass(), dropPosition.ImpactPoint + (dropPosition.ImpactNormal * 25.0f),
            GetActorForwardVector().Rotation());
    }
    else
    {
        FTransform spawnTransform;

        spawnTransform.SetRotation((GetActorForwardVector().Rotation().Quaternion()));
        spawnTransform.SetLocation(dropPosition.TraceEnd);

        ASKCollectible *SpawnedItem =
            GetWorld()->SpawnActorDeferred<ASKCollectible>(ItemToRemove->GetItemClass(), spawnTransform);
        if (SpawnedItem)
        {
            SpawnedItem->FinishSpawning(spawnTransform);
        }
        else
        {
            UE_LOGFMT(LogSKInteractions, Error, "{1} tried to drop {2} but there was an error during spawning!",
                      ("1", GetName()), ("2", ItemToRemove->GetName()));
        }
    }
}

bool ASKPlayerCharacter::CanGrabItem()
{
    if (!InteractionComponent->GetInteractionTarget()) return false;

    return InteractionComponent->GetInteractionTarget()->GetRootComponent()->IsSimulatingPhysics();
}

void ASKPlayerCharacter::HandleGrabbing()
{
    const auto tagGrabbingItem = FSKGameplayTags::Get().Character_State_Action_GrabbingItem;
    const auto tagRotatingItem = FSKGameplayTags::Get().Character_State_Action_RotatingItem;

    if (CanGrabItem())
    {
        AbilitySystemComponent->CheckAndAddGameplayTag(tagGrabbingItem);
        PhysicsHandle->GrabItem(InteractionComponent->GetInteractionTarget()->GetComponentByClass<UMeshComponent>());
    }
    else if (!CanGrabItem() && PhysicsHandle->GetGrabbedComponent())
    {
        AbilitySystemComponent->CheckAndRemoveGameplayTag(tagGrabbingItem);
        AbilitySystemComponent->CheckAndRemoveGameplayTag(tagRotatingItem);
        PhysicsHandle->ReleaseItem();
    }
}

void ASKPlayerCharacter::HandleAlternativeAction()
{
    const auto tagGrabbingItem = FSKGameplayTags::Get().Character_State_Action_GrabbingItem;
    const auto tagRotatingItem = FSKGameplayTags::Get().Character_State_Action_RotatingItem;

    if (AbilitySystemComponent->HasMatchingGameplayTag(tagGrabbingItem) &&
        !AbilitySystemComponent->HasMatchingGameplayTag(tagRotatingItem))
    {
        AbilitySystemComponent->AddLooseGameplayTag(tagRotatingItem);
    }
    else if (AbilitySystemComponent->HasMatchingGameplayTag(tagRotatingItem))
    {
        AbilitySystemComponent->RemoveLooseGameplayTag(tagRotatingItem);
    }
    else
    {
        return;
    }
}

/********************* UTILS *********************/

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
                                         const UPrimitiveComponent *ComponentToIgnore)
{
    FVector TraceStart = PlayerCamera->GetComponentLocation();
    FVector TracecEnd = TraceStart + (PlayerCamera->GetForwardVector() * TraceDistance);

    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredComponent(ComponentToIgnore);

    return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TracecEnd, ECollisionChannel::ECC_Visibility,
                                                TraceParams);
}