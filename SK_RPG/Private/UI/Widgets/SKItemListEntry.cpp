// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/Widgets/SKItemListEntry.h"
#include "Components/Button.h"
#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"
#include "UI/Data/SKInventoryObjectData.h"

void USKItemListEntry::NativeOnListItemObjectSet(UObject *ListItemObject)
{
    if (auto DataObject = Cast<USKInventoryObjectData>(ListItemObject))
    {
        if (TXT_Item_Name)
        {
            TXT_Item_Name->SetText(FText::FromName(DataObject->GetItemName()));
        }
        if (TXT_Item_Quantity)
        {
            TXT_Item_Quantity->SetText(FText::AsNumber(DataObject->GetItemQuantity()));
        }
        if (BTN_Drop_Button)
        {
            BTN_Drop_Button->OnClicked.AddDynamic(this, &USKItemListEntry::HandleItemDrop);
        }
        InventoryItemData = DataObject;

        UE_LOGFMT(LogSKInteractions, Display, "Added list entry: \"{0}\", with Item {1} of quantity {2}",
                  this->GetName(), DataObject->GetItemName(), FString::FormatAsNumber(DataObject->GetItemQuantity()));
    }
}

void USKItemListEntry::HandleItemDrop()
{
    UE_LOG(LogTemp, Display, TEXT("Trying to drop"));

    if (OnItemDropCalled.IsBound())
    {
        OnItemDropCalled.Broadcast(this, 1);
    }
}

void USKItemListEntry::UnbindDelegates()
{
    if (BTN_Drop_Button)
    {
        BTN_Drop_Button->OnClicked.RemoveAll(this);
    }
}
