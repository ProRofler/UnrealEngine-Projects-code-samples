// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Controllers/SKPlayerController.h"
#include "Characters/SKPlayerCharacter.h"
#include "Core/SKLogCategories.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Logging/StructuredLog.h"
#include "UI/SKPlayerHUD.h"
#include "UI/Widgets/SKInventoryWidget.h"

void ASKPlayerController::BeginPlay()
{
    Super::BeginPlay();

    ControllerSetup();
    InitializeComponents();
}

void ASKPlayerController::ToggleInventoryHUD()
{
    if (PlayerHUD.IsValid())
    {
        PlayerHUD->ToggleInventoryVisibility();

        if (PlayerHUD->IsInventoryOpen())
        {
            bShowMouseCursor = true;

            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(PlayerHUD->GetInventoryWidget()->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            SetInputMode(InputMode);
        }
        else
        {
            bShowMouseCursor = false;
            SetInputMode(FInputModeGameOnly());
        }
    }
}

void ASKPlayerController::OnPossess(APawn *aPawn)
{

    Super::OnPossess(aPawn);

    if (UEnhancedInputComponent *Input = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        SKPlayerCharacter = CastChecked<ASKPlayerCharacter>(GetCharacter());

        Input->BindAction(InputData.MovingAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::MoveAction);
        Input->BindAction(InputData.MovingAction, ETriggerEvent::Completed, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::MoveAction);
        Input->BindAction(InputData.LookAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::LookingAction);
        Input->BindAction(InputData.LookAction, ETriggerEvent::Completed, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::LookingAction);
        Input->BindAction(InputData.JumpAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::TryJumping);
        Input->BindAction(InputData.SprintAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::TrySprinting);
        Input->BindAction(InputData.WalkAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::TryWalking);
        Input->BindAction(InputData.AltAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::HandleAlternativeAction);
        Input->BindAction(InputData.InteractionAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::Interact);
        Input->BindAction(InputData.InteractionActionHold, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::HandleGrabbing);
        Input->BindAction(InputData.InventoryToggleAction, ETriggerEvent::Triggered, this,
                          &ASKPlayerController::ToggleInventoryHUD);
        Input->BindAction(InputData.DrawWeaponAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::TryDrawWeapon);
        Input->BindAction(InputData.AttackAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::TryAttacking);
        Input->BindAction(InputData.BlockAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::TryBlocking);
    }
}

void ASKPlayerController::ControllerSetup()
{

    if (APlayerController *PC = Cast<APlayerController>(this))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(InputData.InputMapping, 0);

            UE_LOGFMT(LogSKCharacterMovement, Display, "Enhanced input mapping setup successfull");
        }
    }
}

void ASKPlayerController::InitializeComponents() { PlayerHUD = CastChecked<ASKPlayerHUD>(GetHUD()); }
