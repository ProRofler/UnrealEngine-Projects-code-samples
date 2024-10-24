// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/Widgets/SKInventoryWidget.h"
#include "Components/ListView.h"
#include "UI/Data/SKInventoryObjectData.h"

void USKInventoryWidget::NativeConstruct() { Super::NativeConstruct(); }

void USKInventoryWidget::AddToInventoryList(FInventoryItemData& Data)
{
    auto Item = NewObject<USKInventoryObjectData>();
    Item->ItemData.Name = Data.Name;

    InventoryList->AddItem(Item);
}
