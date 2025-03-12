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
        if (BTN_Equip_Button)
        {
            BTN_Equip_Button->OnClicked.AddDynamic(this, &USKItemListEntry::HandleEquipButton);
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
    else
    {
        UE_LOGFMT(LogSKUserInterface, Error, "Function for drop item delegate is not bound!");
    }
}

void USKItemListEntry::HandleEquipButton()
{
    if (OnItemUseCalled.IsBound())
    {
        OnItemUseCalled.Broadcast(this);
    }
    else
    {
        UE_LOGFMT(LogSKUserInterface, Error, "Function for use item delegate is not bound!");
    }
}

void USKItemListEntry::UnbindDelegates()
{
    if (BTN_Drop_Button)
    {
        BTN_Drop_Button->OnClicked.RemoveAll(this);
    }

    if (BTN_Equip_Button)
    {
        BTN_Equip_Button->OnClicked.RemoveAll(this);
    }
}

void USKItemListEntry::ChangeButtonBGColor(const FColor NewColor)
{
    BTN_Equip_Button->SetBackgroundColor(NewColor);

    UE_LOGFMT(LogSKUserInterface, Display, "Item list entry {1} color change call", GetName());
}
