// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/Widgets/SKItemListEntry.h"
#include "Components/Button.h"
#include "UI/Data/SKInventoryObjectData.h"

void USKItemListEntry::NativeOnListItemObjectSet(UObject *ListItemObject)
{
    if (auto DataObject = Cast<USKInventoryObjectData>(ListItemObject))
    {
        if (TXT_Item_Name)
        {
            TXT_Item_Name->SetText(FText::FromName(DataObject->InventoryItemData.Name));
        }
        if (BTN_Drop_Button)
        {
            BTN_Drop_Button->OnClicked.AddDynamic(this, &USKItemListEntry::HandleItemDrop);
        }
        ItemData = DataObject->ItemData;
    }
}

void USKItemListEntry::HandleItemDrop()
{
    if (ItemData && OnItemDropCalled.IsBound())
    {
        OnItemDropCalled.Broadcast(ItemData, this);
    }
}

void USKItemListEntry::UnbindDelegates()
{
    if (BTN_Drop_Button)
    {
        BTN_Drop_Button->OnClicked.RemoveAll(this);
    }
}
