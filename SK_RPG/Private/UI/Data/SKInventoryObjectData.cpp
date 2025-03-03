// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/Data/SKInventoryObjectData.h"

void USKInventoryObjectData::InitializeItemData(const FName &_ItemName, const int32 _ItemQuantity,
                                                const TSubclassOf<ASKCollectible> _ItemClass,
                                                const ECollectibleType _ItemType)
{
    ItemName = _ItemName;
    ItemQuantity = _ItemQuantity;
    ItemClass = _ItemClass;
    ItemType = _ItemType;
}
