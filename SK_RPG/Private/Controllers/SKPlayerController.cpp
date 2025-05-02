// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Controllers/SKPlayerController.h"
#include "Characters/SKPlayerCharacter.h"
#include "Core/Input/SKAbilityInputConfigDataAsset.h"
#include "Core/Input/SKInputComponent.h"
#include "Core/SKLogCategories.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Logging/StructuredLog.h"
#include "UI/SKPlayerHUD.h"
#include "UI/Widgets/SKInventoryWidget.h"

void ASKPlayerController::BeginPlay()
{
    Super::BeginPlay();

    ControllerSetup();
}

void ASKPlayerController::OnPossess(APawn *aPawn)
{

    Super::OnPossess(aPawn);

    InitializeComponents();

    if (USKInputComponent *Input = CastChecked<USKInputComponent>(InputComponent))
    {

        Input->BindAction(InputData.MovingAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::MoveAction);
        Input->BindAction(InputData.MovingAction, ETriggerEvent::Completed, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::MoveAction);
        Input->BindAction(InputData.LookAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::LookingAction);
        Input->BindAction(InputData.LookAction, ETriggerEvent::Completed, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::LookingAction);
        Input->BindAction(InputData.WalkAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::TryWalking);
        Input->BindAction(InputData.AltAction, ETriggerEvent::Triggered, SKPlayerCharacter.Get(),
                          &ASKPlayerCharacter::HandleAlternativeAction);

        Input->BindAbilityActions(InputConfig, this, &ASKPlayerController::AbilityInputTagPressed,
                                  &ASKPlayerController::AbilityInputTagReleased,
                                  &ASKPlayerController::AbilityInputTagHeld);
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

void ASKPlayerController::InitializeComponents()
{
    SKPlayerCharacter = CastChecked<ASKPlayerCharacter>(GetCharacter());
    PlayerHUD = CastChecked<ASKPlayerHUD>(GetHUD());
    SKAbilitySystemComponent = CastChecked<USKAbilitySystemComponent>(SKPlayerCharacter->GetAbilitySystemComponent());
}

void ASKPlayerController::ToggleCursor()
{

    if (bIsInCursorMode == false)
    {
        bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
        SetInputMode(InputMode);
        SetIgnoreLookInput(true);

        bIsInCursorMode = true;
    }
    else
    {
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
        SetIgnoreLookInput(false);

        bIsInCursorMode = false;
    }
}

void ASKPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (SKAbilitySystemComponent.IsValid()) SKAbilitySystemComponent.Get()->AbilityInputTagPressed(InputTag);
}

void ASKPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (SKAbilitySystemComponent.IsValid()) SKAbilitySystemComponent.Get()->AbilityInputTagReleased(InputTag);
}

void ASKPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (SKAbilitySystemComponent.IsValid()) SKAbilitySystemComponent.Get()->AbilityInputTagHeld(InputTag);
}
