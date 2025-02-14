// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/SKBaseCharacter.h"
#include "Core/Interface/SKInterfaceInteractable.h"
#include "Core/SKLogCategories.h"
#include "Gameplay/Interactables/SKCollectible.h"
#include "Logging/StructuredLog.h"
#include "UI/Data/SKInventoryObjectData.h"

/********************* DEFAULT *********************/
USKInventoryComponent::USKInventoryComponent() { PrimaryComponentTick.bCanEverTick = false; }

void USKInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    OwningCharacter = GetOwningCharacter();
    // InitDelegates();
}

/********************* Items handling *********************/
void USKInventoryComponent::AddToInventory(AActor *PickedItem)
{
    auto itemData = CreateInventoryObjectDataItem(PickedItem);

    if (auto item = FindInventoryItem(itemData))
    {
        UE_LOG(LogTemp, Display, TEXT("Found in inventory"));
        item->IncreaseItemQuantity(itemData->GetItemQuantity());
    }
    else
    {
        InventoryData.Add(itemData);
        UE_LOG(LogTemp, Display, TEXT("Not found in inventory"));
    }

    SortInventory();

    if (OnInventoryUpdated.IsBound())
    {
        OnInventoryUpdated.Execute();
    }
    else
    {
        UE_LOG(LogSKInteractions, Error, TEXT("Item pickup delegate has failed!"));
        return;
    }

    UE_LOGFMT(LogSKInteractions, Display, "Actor: {0} picked up item: \"{1}\", of class: {2}",
              OwningCharacter->GetName(), itemData->GetItemName(), itemData->GetItemClass()->GetName());
}

void USKInventoryComponent::RemoveFromInventory(USKInventoryObjectData *ItemToRemove, const int32 QuantityToDrop)
{
    if (!ItemToRemove) return;

    if (ItemToRemove->GetItemQuantity() - QuantityToDrop <= 0)
    {

        if (OnItemPickup.IsBound())
        {
            OnItemPickup.RemoveAll(ItemToRemove);
        }

        InventoryData.Remove(ItemToRemove);
    }
    else
    {
        auto i = InventoryData.Find(ItemToRemove);
        InventoryData[i]->DecreaseItemQuantity(QuantityToDrop);
    }

    OnInventoryUpdated.ExecuteIfBound();

    UE_LOGFMT(LogSKInteractions, Display, "Actor: {0} Dropped item: {1}", this->GetName(), ItemToRemove->GetName());
}

TObjectPtr<USKInventoryObjectData> USKInventoryComponent::CreateInventoryObjectDataItem(const AActor *Item)
{
    if (!Item) return nullptr;

    auto originaItem = Cast<ASKCollectible>(Item);
    if (!originaItem) return nullptr;

    if (TObjectPtr<USKInventoryObjectData> newInventoryItem = NewObject<USKInventoryObjectData>(this))
    {
        newInventoryItem->InitializeItemData(originaItem->GetInGameName(), originaItem->GetItemQuantity(),
                                             originaItem->GetClass());
        return newInventoryItem;
    }
    else
    {
        UE_LOGFMT(LogSKInteractions, Display, "Actor: {0} failed to generate inventory item: \"{1}\", of class: {2}",
                  OwningCharacter->GetName(), originaItem->GetName(), originaItem->GetClass()->GetName());
        return nullptr;
    }
}

void USKInventoryComponent::SortInventory()
{
    UE_LOG(LogTemp, Display, TEXT("Attempting to sort the inventory"));

    InventoryData.Sort([](const TObjectPtr<USKInventoryObjectData> &A, const TObjectPtr<USKInventoryObjectData> &B) {
        if (A && B)
        {
            return A->GetItemName().LexicalLess(B->GetItemName());
        }
        else
        {
            return A != nullptr;
        }
    });
}

/********************* Initialization *********************/
ASKBaseCharacter *USKInventoryComponent::GetOwningCharacter()
{
    auto owner = GetOwner();
    auto castToChar = Cast<ASKBaseCharacter>(owner);
    ensureMsgf(castToChar, TEXT("No owner found!"));
    return castToChar;
}

void USKInventoryComponent::InitDelegates()
{
    // OwningCharacter->OnItemPickup.AddDynamic(this, &USKInventoryComponent::AddToInventory);
}

USKInventoryObjectData *USKInventoryComponent::FindInventoryItem(const USKInventoryObjectData *ObjectData)
{
    for (auto &Data : InventoryData)
    {
        check(Data)

            if (ObjectData->GetItemClass() == Data->GetItemClass())
        {
            return Data.Get();
        }
    }
    return nullptr;
}
