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

    PlayerUIWidget = CastChecked<USKUserWidgetBase>(CreateWidget(GetWorld(), PlayerUIWidgetClass));
    SKPlayer = CastChecked<ASKPlayerCharacter>(GetOwningPlayerController()->GetCharacter());

    SKPlayer->OnCharacterDeath.AddDynamic(this, &ASKPlayerHUD::HandlePlayerDeath);

    PlayerUIWidget->AddToViewport(10);
}

void ASKPlayerHUD::HandlePlayerDeath() { PlayerUIWidget->RemoveFromViewport(); }

// void ASKPlayerHUD::DrawHUD() { Super::DrawHUD(); }