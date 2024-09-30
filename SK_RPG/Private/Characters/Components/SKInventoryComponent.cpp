// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKInventoryComponent.h"

//********************* DEFAULT *********************
USKInventoryComponent::USKInventoryComponent() { PrimaryComponentTick.bCanEverTick = false; }

void USKInventoryComponent::BeginPlay() { Super::BeginPlay(); }

//********************* Items handling *********************
void USKInventoryComponent::AddToInventory(const TObjectPtr<AActor> Item)
{
    if (InventoryData.Find(Item) == INDEX_NONE)
    {
        InventoryData.Add(Item);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Dupe item no logic yet"));
        // combining logic
    }
}

void USKInventoryComponent::RemoveFromInventory(const TObjectPtr<AActor> Item) {}
