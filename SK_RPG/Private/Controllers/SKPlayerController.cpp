// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Controllers/SKPlayerController.h"
#include "UI/SKPlayerHUD.h"
#include "UI/Widgets/SKInventoryWidget.h"

void ASKPlayerController::BeginPlay()
{
    Super::BeginPlay();
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

void ASKPlayerController::InitializeComponents()
{
    PlayerHUD = Cast<ASKPlayerHUD>(GetHUD());
    check(PlayerHUD.Get());
}
