// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Characters/Components/SKCharacterComponentBase.h"

#include "CoreMinimal.h"
#include "UI/Data/SKInventoryObjectData.h"

#include "SKInventoryComponent.generated.h"

class USKInventoryObjectData;
class ASKBaseCharacter;
class ASKCollectible;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickupDelegateSignature, const AActor *, PickedItem);
DECLARE_DYNAMIC_DELEGATE(FOnInventoryUpdatedSignature);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIRKNIGHT_API USKInventoryComponent : public USKCharacterComponentBase
{
    GENERATED_BODY()
    friend ASKBaseCharacter;

    //
    /********************* DEFAULT *********************/
  public:
    USKInventoryComponent();

    //
    /********************* Getters *********************/
  public:
    UFUNCTION(BlueprintPure, Category = "SK Inventory Component|Weapon slots")
    FORCEINLINE TArray<USKInventoryObjectData *> &GetInventoryData() { return InventoryData; }

    UFUNCTION(BlueprintPure, Category = "SK Inventory Component|Weapon slots")
    FORCEINLINE USKInventoryObjectData *GetMainWeaponSlot() { return MainWeaponSlot; }

    //
    /********************* Equip handling *********************/
  public:
    UFUNCTION(BlueprintCallable, Category = "SK Weapon Component")
    void HandleEquip(USKInventoryObjectData *ObjectData);

  private:
    void EquipWeapon(USKInventoryObjectData *ObjectData);

    //
    /********************* Inventory handle *********************/
  public:
    UFUNCTION(BlueprintCallable, Category = "SK Inventory Component")
    void AddToInventory(AActor *PickedItem);

    bool RemoveFromInventory(USKInventoryObjectData *ItemToRemove, const int32 QuantityToDrop);

    //
    /********************* Invevntory search *********************/
  public:
    UFUNCTION(BlueprintCallable, Category = "SK Inventory Component")
    bool IsInventoryEmpty() const { return InventoryData.IsEmpty(); }

    USKInventoryObjectData *FindByClass(const TSubclassOf<ASKCollectible> &CollectibleClass) const;

  private:
    USKInventoryObjectData *FindByObjectData(USKInventoryObjectData *ObjectData);

    //
    /********************* UTILS *********************/
  private:
    USKInventoryObjectData *CreateInventoryObjectDataItem(const AActor *Item);
    USKInventoryObjectData *SplitInventoryObjectData(USKInventoryObjectData *ObjectData, const uint32 SplitAmount);
    void SortInventory();

    void SetMainWeaponSlot(USKInventoryObjectData *ObjectData) { MainWeaponSlot = ObjectData; }

    //
    /********************* Members *********************/
  public:
    // DELEGATES
    UPROPERTY(BlueprintAssignable, Category = "SK Inventory Component|Events")
    FOnItemPickupDelegateSignature OnItemPickup;
    UPROPERTY(BlueprintReadOnly, Category = "SK Inventory Component|Events")
    FOnInventoryUpdatedSignature OnInventoryUpdated;

  private:
    UPROPERTY()
    TArray<USKInventoryObjectData *> InventoryData; // Main inventory data

    UPROPERTY()
    TObjectPtr<USKInventoryObjectData> MainWeaponSlot = nullptr;
};