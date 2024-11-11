// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/Widgets/SKInventoryWidget.h"
#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/SKPlayerCharacter.h"
#include "Components/ListView.h"
#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"
#include "Props/SKCollectible.h"
#include "UI/Data/SKInventoryObjectData.h"
#include "UI/Widgets/SKItemListEntry.h"

void USKInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    Player = GetOwningCharacter();
    InitDelegates();
}

ASKPlayerCharacter *USKInventoryWidget::GetOwningCharacter()
{
    auto owner = GetOwningPlayer();
    if (!owner) return nullptr;

    auto castToChar = Cast<ASKPlayerCharacter>(owner->GetPawn());
    ensureMsgf(castToChar, TEXT("No owner found!"));

    return castToChar;
}

/********************** FRONT ***********************/
void USKInventoryWidget::UpdateInventoryWidget()
{

    if (!InventoryList) return;

    InventoryList->ClearListItems();

    for (auto itemData : Player->GetInventoryComponent()->GetInventoryData())
    {
        if (itemData.IsValid())
        {
            auto listEntryObjetData = CreateInventoryObjectDataItem(itemData.Get());
            if (!listEntryObjetData) return;

            InventoryList->AddItem(listEntryObjetData);
        }
    }

    // LOG
    UE_LOG(LogSKUserInterface, Display, TEXT("Inventory list was succesfully updated"));
}

/********************** BACK ***********************/
void USKInventoryWidget::HandleDropItem(AActor *ItemData, const USKItemListEntry *ListEntry)
{
    if (!ItemData || !ListEntry) return;

    Player->DropItem(ItemData);
    UpdateInventoryWidget();
    bIsPendingUpdate = false;

    UE_LOGFMT(LogSKInteractions, Display,
              "Item: {ItemDataName} drop call, from ListEntry: {ListEntryName} by: {ActorName}",
              ("ItemDataName", ItemData->GetName()), ("ListEntryName", ListEntry->GetName()),
              ("ActorName", Player->GetFName()));
}

void USKInventoryWidget::HandleInventoryOpen()
{
    if (bIsPendingUpdate)
    {
        UpdateInventoryWidget();
        bIsPendingUpdate = false;
    }
}

TObjectPtr<USKInventoryObjectData> USKInventoryWidget::CreateInventoryObjectDataItem(const AActor *Item)
{
    if (!Item) return nullptr;

    TObjectPtr<USKInventoryObjectData> newInventoryItem = NewObject<USKInventoryObjectData>(this);
    newInventoryItem->ItemData = const_cast<AActor *>(Item);

    FInventoryItemData inventoryItemData;
    auto originaItem = Cast<ASKCollectible>(Item);
    if (!originaItem) return nullptr;

    inventoryItemData.Name = originaItem->GetInGameName();
    newInventoryItem->InventoryItemData = inventoryItemData;

    return newInventoryItem;

    return TObjectPtr<USKInventoryObjectData>();
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
    Player->GetInventoryComponent()->OnInventoryUpdated.BindDynamic(this, &USKInventoryWidget::HandleInventoryChanged);
    InventoryList->OnEntryWidgetGenerated().AddUObject(this, &USKInventoryWidget::HandleEntryWidgetGenerated);
    InventoryList->OnEntryWidgetReleased().AddUObject(this, &USKInventoryWidget::HandleEntryWidgetReleased);
}