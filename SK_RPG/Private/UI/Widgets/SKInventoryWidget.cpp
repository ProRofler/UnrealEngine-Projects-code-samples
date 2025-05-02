// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/Widgets/SKInventoryWidget.h"

#include "Core/Interface/SKInterfaceCharacter.h"

#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/SKPlayerCharacter.h"

#include "Gameplay/Interactables/SKCollectible.h"

#include "UI/Data/SKInventoryObjectData.h"
#include "UI/SKPlayerHUD.h"
#include "UI/Widgets/SKListViewWidget.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

void USKInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    InitDelegates();
}

void USKInventoryWidget::HandleInventoryOpen()
{
    if (bIsPendingUpdate)
    {
        WeaponsListViewWidget->UpdateListViewWidget();
        KeysListViewWidget->UpdateListViewWidget();
        MiscListViewWidget->UpdateListViewWidget();
        bIsPendingUpdate = false;
    }
}

void USKInventoryWidget::HandleInventoryListUpdate()
{
    if (IsVisible())
    {
        WeaponsListViewWidget->UpdateListViewWidget();
        KeysListViewWidget->UpdateListViewWidget();
        MiscListViewWidget->UpdateListViewWidget();
    }

    bIsPendingUpdate = true;
}

/********************** UTILS ***********************/
void USKInventoryWidget::InitDelegates()
{
    ISKInterfaceCharacter::Execute_GetInventoryComponent(GetSKPlayerHud()->GetSKPlayerCharacter())
        ->OnInventoryUpdated.BindDynamic(this, &USKInventoryWidget::HandleInventoryListUpdate);
}