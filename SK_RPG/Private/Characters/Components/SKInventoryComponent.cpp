// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/SKBaseCharacter.h"
#include "Core/Interface/SKInterfaceInteractable.h"

#include "Gameplay/Interactables/SKCollectible.h"
#include "Gameplay/Interactables/SKKeyItem.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

/********************* DEFAULT *********************/
USKInventoryComponent::USKInventoryComponent() { PrimaryComponentTick.bCanEverTick = false; }

//
/********************* Equip handling *********************/

void USKInventoryComponent::HandleEquip(USKInventoryObjectData *ObjectData)
{
    if (!ObjectData) return;

    if (ObjectData->GetItemType() == ECollectibleType::Weapon)
    {
        EquipWeapon(ObjectData);
    }

    OnInventoryUpdated.ExecuteIfBound();
}

void USKInventoryComponent::EquipWeapon(USKInventoryObjectData *ObjectData)
{

    if (ObjectData == GetMainWeaponSlot())
    {
        if (auto item = FindByClass(ObjectData->GetItemClass()))
        {
            item->ChangeItemQuantity(ObjectData->GetItemQuantity());
        }
        else
        {
            InventoryData.Add(ObjectData);
        }

        SetMainWeaponSlot(nullptr);
    }
    else
    {
        if (GetMainWeaponSlot())
        {
            if (auto item = FindByClass(GetMainWeaponSlot()->GetItemClass()))
            {
                item->ChangeItemQuantity(GetMainWeaponSlot()->GetItemQuantity());
            }
            else
            {
                InventoryData.Add(GetMainWeaponSlot());
            }

            SetMainWeaponSlot(nullptr);
        }

        auto itemToEquip = SplitInventoryObjectData(ObjectData, 1);
        SetMainWeaponSlot(itemToEquip);
    }
}

//
/********************* Invevntory handle *********************/

void USKInventoryComponent::AddToInventory(AActor *PickedItem)
{
    if (!PickedItem) return;

    auto itemData = CreateInventoryObjectDataItem(PickedItem);
    if (!itemData)
    {
        UE_LOGFMT(LogSKInteractions, Error, "Actor: {0} failed to generate inventory item: \"{1}\", of class: {2}",
                  GetSKOwnerCharacter()->GetName(), PickedItem->GetName(), PickedItem->GetClass()->GetName());

        checkNoEntry();
    }

    if (auto item = FindByObjectData(itemData))
    {
        UE_LOG(LogTemp, Display, TEXT("Found in inventory"));
        item->ChangeItemQuantity(itemData->GetItemQuantity());
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
              GetSKOwnerCharacter()->GetName(), itemData->GetItemName(), itemData->GetItemClass()->GetName());
}

bool USKInventoryComponent::RemoveFromInventory(USKInventoryObjectData *ItemToRemove, const int32 QuantityToRemove)
{
    if (!ItemToRemove) return false;
    if (ItemToRemove == GetMainWeaponSlot()) return false;

    if (ItemToRemove->GetItemQuantity() - QuantityToRemove <= 0)
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
        InventoryData[i]->ChangeItemQuantity(-QuantityToRemove);
    }

    OnInventoryUpdated.ExecuteIfBound();

    UE_LOGFMT(LogSKInteractions, Display, "Item {0} was removed from {1} inventory", ItemToRemove->GetName(),
              GetName());

    return true;
}

//
/********************* Inventory search *********************/

USKInventoryObjectData *USKInventoryComponent::FindByClass(const TSubclassOf<ASKCollectible> &CollectibleClass) const
{
    for (auto item : InventoryData)
    {
        if (item->GetItemClass() == CollectibleClass) return item;
    }

    return nullptr;
}

USKInventoryObjectData *USKInventoryComponent::FindByObjectData(USKInventoryObjectData *ObjectData)
{
    for (auto &Data : InventoryData)
    {
        check(Data)

            if (ObjectData->GetItemClass() == Data->GetItemClass())
        {
            return Data;
        }
    }

    return nullptr;
}

//
/********************* UTILS *********************/

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

USKInventoryObjectData *USKInventoryComponent::CreateInventoryObjectDataItem(const AActor *Item)
{
    auto originaItem = Cast<ASKCollectible>(Item);
    if (!originaItem) return nullptr;

    USKInventoryObjectData *newInventoryItem = NewObject<USKInventoryObjectData>(this);

    newInventoryItem->InitializeItemData(originaItem->GetInteractableName(), originaItem->GetItemQuantity(),
                                         originaItem->GetClass(), originaItem->GetCollectibleType());

    return newInventoryItem;
}

USKInventoryObjectData *USKInventoryComponent::SplitInventoryObjectData(USKInventoryObjectData *ObjectData,
                                                                        const uint32 SplitAmount)
{
    auto objectData = FindByObjectData(ObjectData);
    if (!objectData) return nullptr;

    const uint32 itemQuantity = objectData->GetItemQuantity();

    if (SplitAmount > itemQuantity) return nullptr;

    if (itemQuantity == 1)
    {
        RemoveFromInventory(objectData, 1);
        if (!OnInventoryUpdated.ExecuteIfBound())
        {
            UE_LOG(LogSKInteractions, Error, TEXT("Inventory update delegate has failed!"));
        }
        return objectData;
    }

    auto newObjectData = NewObject<USKInventoryObjectData>();
    newObjectData->InitializeItemData(ObjectData->GetItemName(), SplitAmount, ObjectData->GetItemClass(),
                                      ObjectData->GetItemType());

    objectData->ChangeItemQuantity(-(int32)SplitAmount);

    if (!OnInventoryUpdated.ExecuteIfBound())
    {
        UE_LOG(LogSKInteractions, Error, TEXT("Inventory update delegate has failed!"));
    }

    return newObjectData;
}