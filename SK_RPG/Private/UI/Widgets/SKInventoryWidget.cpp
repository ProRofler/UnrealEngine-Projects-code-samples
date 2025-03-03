// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/Widgets/SKInventoryWidget.h"

#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/SKPlayerCharacter.h"

#include "Components/ListView.h"

#include "Gameplay/Interactables/SKCollectible.h"

#include "UI/Data/SKInventoryObjectData.h"
#include "UI/SKPlayerHUD.h"
#include "UI/Widgets/SKItemListEntry.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

#include "Core/Interface/SKInterfaceCharacter.h"

void USKInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    InitDelegates();
}

/********************** FRONT ***********************/
void USKInventoryWidget::UpdateInventoryWidget()
{

    if (!InventoryList) return;

    InventoryList->ClearListItems();
    InventoryList->RegenerateAllEntries();

    if (InventoryList->GetListItems().IsEmpty())
    {
        const auto playerInventoryData =
            ISKInterfaceCharacter::Execute_GetInventoryComponent(GetSKPlayerHud()->GetSKPlayerCharacter())
                ->GetInventoryData();

        for (const auto &itemData : playerInventoryData)
        {
            if (itemData)
            {
                InventoryList->AddItem(itemData);
                UE_LOGFMT(LogSKInteractions, Display, "Parsed item: \"{1}\", of quantity {2} to list",
                          itemData->GetItemName(), FString::FormatAsNumber(itemData->GetItemQuantity()));
            }
        }
        UE_LOG(LogSKUserInterface, Display, TEXT("Inventory list was succesfully updated"));
    }
    else
    {
        UE_LOG(LogSKUserInterface, Error, TEXT("Inventory list wasn't empty before populating"));
    }
}

/********************** BACK ***********************/
void USKInventoryWidget::HandleDropItem(const USKItemListEntry *ListEntry, const int32 QuantityToDrop)
{
    if (!ListEntry) return;

    const auto player = GetSKPlayerHud()->GetSKPlayerCharacter();

    player->DropItem(ListEntry->GetInventoryItemData(), QuantityToDrop);
    UpdateInventoryWidget();
    bIsPendingUpdate = false;

    UE_LOGFMT(LogSKInteractions, Display,
              "Item: {ItemDataName} drop call, from ListEntry: {ListEntryName} by: {ActorName}",
              ("ItemDataName", ListEntry->GetInventoryItemData()->GetItemName()),
              ("ListEntryName", ListEntry->GetName()), ("ActorName", player->GetFName()));
}

void USKInventoryWidget::HandleInventoryOpen()
{
    if (bIsPendingUpdate)
    {
        UpdateInventoryWidget();
        bIsPendingUpdate = false;
    }
}

void USKInventoryWidget::HandleEntryWidgetGenerated(UUserWidget &EntryWidget)
{
    const auto listEntry = Cast<USKItemListEntry>(&EntryWidget);
    if (!listEntry) return;
    listEntry->OnItemDropCalled.AddDynamic(this, &USKInventoryWidget::HandleDropItem);
}

void USKInventoryWidget::HandleEntryWidgetReleased(UUserWidget &EntryWidget)
{
    const auto listEntry = Cast<USKItemListEntry>(&EntryWidget);
    if (!listEntry) return;
    listEntry->UnbindDelegates();
    listEntry->OnItemDropCalled.RemoveAll(this);
}

/********************** UTILS ***********************/
void USKInventoryWidget::InitDelegates()
{
    ISKInterfaceCharacter::Execute_GetInventoryComponent(GetSKPlayerHud()->GetSKPlayerCharacter())
        ->OnInventoryUpdated.BindDynamic(this, &USKInventoryWidget::MarkForUpdate);

    InventoryList->OnEntryWidgetGenerated().AddUObject(this, &USKInventoryWidget::HandleEntryWidgetGenerated);
    InventoryList->OnEntryWidgetReleased().AddUObject(this, &USKInventoryWidget::HandleEntryWidgetReleased);
}