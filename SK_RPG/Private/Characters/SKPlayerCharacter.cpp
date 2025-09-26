// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/SKPlayerCharacter.h"

#include "Camera/CameraComponent.h"

#include "Characters/Components/SKInteractionComponent.h"
#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/Components/SKPhysicsHandleComponent.h"

#include "Components/CapsuleComponent.h"

#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Gameplay/GAS/SKNativeGameplayTags.h"

#include "UI/Data/SKInventoryObjectData.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

#include "Controllers/SKPlayerController.h"
#include "EnhancedInputComponent.h"

ASKPlayerCharacter::ASKPlayerCharacter(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    PlayerCamera = CreateDefaultSubobject<UCameraComponent>("Player camera");
    PlayerCamera->SetupAttachment(GetRootComponent());
    PlayerCamera->bUsePawnControlRotation = true;

    Body = CreateDefaultSubobject<USkeletalMeshComponent>("Player body mesh");
    Body->SetupAttachment(GetRootComponent());
    Body->bUseAttachParentBound = true; // body mesh will be culled out otherwise

    PhysicsHandle = CreateDefaultSubobject<USKPhysicsHandleComponent>("Physics handle");

    InteractionComponent = CreateDefaultSubobject<USKInteractionComponent>("Interaction component");
    InteractionComponent->GetInteractionZone()->SetupAttachment(GetRootComponent());
}
/************************************ UE INHERITED ******************************************/

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

/********************* INTERACTIONS *********************/

void ASKPlayerCharacter::DropItem(USKInventoryObjectData *ItemToRemove, const int32 QuantityToDrop)
{
    if (!ItemToRemove) return;

    if (!InventoryComponent->RemoveFromInventory(ItemToRemove, QuantityToDrop)) return;

    FHitResult dropPosition;

    if (TraceFromCamera(dropPosition, 150.0f))
    {
        GetWorld()->SpawnActor<AActor>(ItemToRemove->GetItemClass(),
                                       dropPosition.ImpactPoint + (dropPosition.ImpactNormal * 25.0f),
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

bool ASKPlayerCharacter::TraceFromCamera(FHitResult &HitResult, const float TraceDistance,
                                         const UPrimitiveComponent *ComponentToIgnore)
{
    FVector TraceStart = PlayerCamera->GetComponentLocation();
    FVector TracecEnd = TraceStart + (PlayerCamera->GetForwardVector() * TraceDistance);

    FCollisionQueryParams TraceParams;
    if (ComponentToIgnore)
    {
        TraceParams.AddIgnoredComponent(ComponentToIgnore);
        TraceParams.AddIgnoredActor(this);
    }

    return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TracecEnd, ECollisionChannel::ECC_Visibility,
                                                TraceParams);
}