// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKInventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "Characters/SKBaseCharacter.h"

#include "Core/Interface/SKInterfaceInteractable.h"

#include "Gameplay/GAS/SKNativeGameplayTags.h"
#include "Gameplay/Interactables/SKCollectible.h"
#include "Gameplay/Interactables/SKKeyItem.h"

#include "Utils/DataTables/SKInventoryDataTable.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

/********************* DEFAULT *********************/
USKInventoryComponent::USKInventoryComponent() { PrimaryComponentTick.bCanEverTick = false; }

void USKInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetSKOwnerCharacter()->StartingInventoryData)
    {
        AddToInventoryFromDataTable(GetSKOwnerCharacter()->StartingInventoryData.Get());
    }
}

//
/********************* Equip handling *********************/

void USKInventoryComponent::HandleUseItem(USKInventoryObjectData *ObjectData)
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
    // TODO: checker function for slot assignment?
    FGameplayEventData payloadTemp;

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

        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
            GetSKOwnerCharacter(), FSKGameplayTags::Get().Event_Combat_SwitchWeapon, payloadTemp);
    }
    else
    {
        if (GetMainWeaponSlot())
        {
            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
                GetSKOwnerCharacter(), FSKGameplayTags::Get().Event_Combat_SwitchWeapon, payloadTemp);

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

    USKInventoryObjectData *itemData = CreateInventoryObjectDataItem(PickedItem);

    if (!itemData)
    {
        UE_LOGFMT(LogSKInteractions, Error, "Actor: {0} failed to generate inventory item: \"{1}\", of class: {2}",
                  GetSKOwnerCharacter()->GetName(), PickedItem->GetName(), PickedItem->GetClass()->GetName());

        checkNoEntry();
    }

    if (auto item = FindByObjectData(itemData))
    {
        item->ChangeItemQuantity(itemData->GetItemQuantity());
    }
    else
    {
        InventoryData.Add(itemData);
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

    if (bEnableLogging)
    {
        UE_LOGFMT(LogSKInteractions, Display, "Actor: {0} picked up item: \"{1}\", of class: {2}",
                  GetSKOwnerCharacter()->GetName(), itemData->GetItemName(), itemData->GetItemClass()->GetName());
    }
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

void USKInventoryComponent::AddToInventoryFromDataTable(const UDataTable *DT)
{
    const FString ContextString(TEXT("StartupInventory"));

    TArray<FSKInventoryDataTable *> AllRow;
    DT->GetAllRows<FSKInventoryDataTable>(ContextString, AllRow);

    for (auto *Row : AllRow)
    {
        if (Row)
        {
            auto item = NewObject<ASKCollectible>(GetOwner(), Row->ItemClass);
            Row->Amount > 0 ? item->SetItemQuantity(Row->Amount) : item->SetItemQuantity(1);

            AddToInventory(item);

            if (Row->MainWeaponSlot && !MainWeaponSlot && (item->GetCollectibleType() == ECollectibleType::Weapon))
            {
                EquipWeapon(CreateInventoryObjectDataItem(item));
            }
            item->Destroy();
        }
    }
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