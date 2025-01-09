// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/SKPlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Core/SKLogCategories.h"
#include "Engine/Canvas.h"
#include "UI/Widgets/SKInventoryWidget.h"

void ASKPlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    InventoryWidget = Cast<USKInventoryWidget>(CreateWidget(GetWorld(), InventoryWidgetClass));

    if (InventoryWidget)
    {
        InventoryWidget->AddToViewport();
        InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        UE_LOG(LogSKUserInterface, Error, TEXT("Player's inventory widget failed to initialize in ASKPlayerHUD class"));
    }
}

void ASKPlayerHUD::DrawHUD()
{
    Super::DrawHUD();
    DrawCrosshair();
}

void ASKPlayerHUD::DrawCrosshair()
{
    TInterval<float> ScreenCenter(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
    // Draw horizontal line
    DrawLine(ScreenCenter.Min - 10, ScreenCenter.Max, ScreenCenter.Min + 10, ScreenCenter.Max, FLinearColor::Blue,
             2.0f);
    // Draw horizontal line
    DrawLine(ScreenCenter.Min, ScreenCenter.Max - 10, ScreenCenter.Min, ScreenCenter.Max + 10, FLinearColor::Blue,
             2.0f);
}

void ASKPlayerHUD::ToggleInventoryVisibility()
{
    if (InventoryWidget)
    {
        switch (bIsInventoryOpen)
        {
        case (0):
            InventoryWidget->HandleInventoryOpen();
            InventoryWidget->SetVisibility(ESlateVisibility::Visible);
            break;
        default: InventoryWidget->SetVisibility(ESlateVisibility::Hidden); break;
        }

        bIsInventoryOpen = !bIsInventoryOpen;
    }
}
