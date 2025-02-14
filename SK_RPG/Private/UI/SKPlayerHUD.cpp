// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/SKPlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Characters/SKPlayerCharacter.h"
#include "Core/SKLogCategories.h"
#include "Engine/Canvas.h"
#include "UI/Widgets/SKInventoryWidget.h"
#include "UI/Widgets/SKMainAttributesWidget.h"

void ASKPlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    InventoryWidget = CastChecked<USKInventoryWidget>(CreateWidget(GetWorld(), InventoryWidgetClass));
    MainAttributesWidget = CastChecked<USKMainAttributesWidget>(CreateWidget(GetWorld(), MainAttributesWidgettClass));
    SKPlayer = CastChecked<ASKPlayerCharacter>(GetOwningPlayerController()->GetCharacter());

    if (InventoryWidget && MainAttributesWidget)
    {
        InventoryWidget->AddToViewport(10);
        InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

        MainAttributesWidget->AddToViewport(0);
        MainAttributesWidget->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        UE_LOG(LogSKUserInterface, Error, TEXT("One of the widgets is nullptr!"));
    }
}

void ASKPlayerHUD::DrawHUD()
{
    Super::DrawHUD();
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
        default: InventoryWidget->SetVisibility(ESlateVisibility::Collapsed); break;
        }

        bIsInventoryOpen = !bIsInventoryOpen;
    }
}
