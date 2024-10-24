// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/Widgets/SKItemListEntry.h"
#include "UI/Data/SKInventoryObjectData.h"

void USKItemListEntry::NativeOnListItemObjectSet(UObject *ListItemObject)
{
    if (auto DataObject = Cast<USKInventoryObjectData>(ListItemObject))
    {
        if (TXT_Item_Name)
        {
            TXT_Item_Name->SetText(FText::FromName(DataObject->ItemData.Name));
        }
    }
}