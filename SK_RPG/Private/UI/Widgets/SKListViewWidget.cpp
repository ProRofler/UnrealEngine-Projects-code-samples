// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/Widgets/SKListViewWidget.h"

#include "Components/ListView.h"
#include "Core/Interface/SKInterfaceCharacter.h"

#include "UI/SKPlayerHUD.h"
#include "UI/Widgets/SKItemListEntry.h"

#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/SKPlayerCharacter.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

void USKListViewWidget::NativeConstruct()
{
    Super::NativeConstruct();
    InitDelegates();
}

void USKListViewWidget::UpdateListViewWidget()
{
    if (!InventoryList) return;

    InventoryList->ClearListItems();
    InventoryList->RegenerateAllEntries();

    const auto playerInventory =
        ISKInterfaceCharacter::Execute_GetInventoryComponent(GetSKPlayerHud()->GetSKPlayerCharacter());

    if (!playerInventory) return;

    if (!InventoryList->GetListItems().IsEmpty())
    {
        UE_LOG(LogSKUserInterface, Error, TEXT("Inventory list wasn't empty before populating"));
        checkNoEntry();
    }

    // Handle slots
    if (ViewCollectibleType == ECollectibleType::Weapon && playerInventory->GetMainWeaponSlot())
    {
        InventoryList->AddItem(playerInventory->GetMainWeaponSlot());
    }

    // Handle main Inventory
    for (const auto &itemData : playerInventory->GetInventoryData())
    {
        if (itemData && itemData->GetItemType() == ViewCollectibleType)
        {
            InventoryList->AddItem(itemData);
            UE_LOGFMT(LogSKInteractions, Display, "Parsed item: \"{1}\", of quantity {2} to list",
                      itemData->GetItemName(), FString::FormatAsNumber(itemData->GetItemQuantity()));
        }
    }

    UE_LOG(LogSKUserInterface, Display, TEXT("Inventory list was succesfully updated"));
}

void USKListViewWidget::InitDelegates()
{
    InventoryList->OnEntryWidgetGenerated().AddUObject(this, &USKListViewWidget::HandleEntryWidgetGenerated);
    InventoryList->OnEntryWidgetReleased().AddUObject(this, &USKListViewWidget::HandleEntryWidgetReleased);
}

void USKListViewWidget::HandleEntryWidgetGenerated(UUserWidget &EntryWidget)
{
    const auto listEntry = Cast<USKItemListEntry>(&EntryWidget);
    if (!listEntry) return;
    listEntry->OnItemDropCalled.AddDynamic(this, &USKListViewWidget::HandleDropItem);
    listEntry->OnItemUseCalled.AddDynamic(this, &USKListViewWidget::HandleUseItem);

    // Paint item to see if equipped
    const auto playerInventory =
        ISKInterfaceCharacter::Execute_GetInventoryComponent(GetSKPlayerHud()->GetSKPlayerCharacter());

    if (playerInventory && ViewCollectibleType == ECollectibleType::Weapon)
    {
        if (listEntry->GetInventoryItemData() == playerInventory->GetMainWeaponSlot())
        {
            listEntry->ChangeButtonBGColor(FColor::FromHex("006C16D9"));
        }
        else
        {
            listEntry->ChangeButtonBGColor(FColor::FromHex("26262652"));
        }
    }
}

void USKListViewWidget::HandleEntryWidgetReleased(UUserWidget &EntryWidget)
{
    const auto listEntry = Cast<USKItemListEntry>(&EntryWidget);
    if (!listEntry) return;

    listEntry->UnbindDelegates();
    listEntry->OnItemDropCalled.RemoveAll(this);
    listEntry->OnItemUseCalled.RemoveAll(this);

    listEntry->Destruct();
}

void USKListViewWidget::HandleDropItem(const USKItemListEntry *ListEntry, const int32 QuantityToDrop)
{
    if (!ListEntry) return;

    const auto player = GetSKPlayerHud()->GetSKPlayerCharacter();
    player->DropItem(ListEntry->GetInventoryItemData(), QuantityToDrop);

    UE_LOGFMT(LogSKInteractions, Display,
              "Item: {ItemDataName} drop call, from ListEntry: {ListEntryName} by: {ActorName}",
              ("ItemDataName", ListEntry->GetInventoryItemData()->GetItemName()),
              ("ListEntryName", ListEntry->GetName()), ("ActorName", player->GetFName()));
}

void USKListViewWidget::HandleUseItem(const USKItemListEntry *ListEntry)
{
    UE_LOGFMT(LogSKUserInterface, Display, "Attempt to use inventory item");

    const auto player = GetSKPlayerHud()->GetSKPlayerCharacter();
    player->HandleUseItem(ListEntry->GetInventoryItemData());
}