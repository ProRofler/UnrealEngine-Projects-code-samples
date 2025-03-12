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

  public:
    USKInventoryComponent();

    UFUNCTION(BlueprintCallable, Category = "SK Inventory Component")
    void AddToInventory(AActor *PickedItem);
    bool RemoveFromInventory(USKInventoryObjectData *ItemToRemove, const int32 QuantityToDrop);
    UFUNCTION(BlueprintCallable, Category = "SK Inventory Component")
    bool IsInventoryEmpty() const { return InventoryData.IsEmpty(); }

    USKInventoryObjectData *FindInInventoryByClass(const TSubclassOf<ASKCollectible> &CollectibleClass) const;

    UPROPERTY(BlueprintAssignable, Category = "SK Inventory Component|Events")
    FOnItemPickupDelegateSignature OnItemPickup;
    UPROPERTY(BlueprintReadOnly, Category = "SK Inventory Component|Events")
    FOnInventoryUpdatedSignature OnInventoryUpdated;

    UFUNCTION(BlueprintPure, Category = "SK Inventory Component|Weapon slots")
    FORCEINLINE TArray<USKInventoryObjectData *> &GetInventoryData() { return InventoryData; }

    UFUNCTION(BlueprintPure, Category = "SK Inventory Component|Weapon slots")
    FORCEINLINE USKInventoryObjectData *GetMainWeaponSlot() { return MainWeaponSlot; }

    UFUNCTION(BlueprintCallable, Category = "SK Weapon Component")
    void HandleEquip(USKInventoryObjectData *ObjectData);

  protected:
    virtual void BeginPlay() override;

  private:
    void SetMainWeaponSlot(USKInventoryObjectData *ObjectData) { MainWeaponSlot = ObjectData; }

    UPROPERTY()
    TArray<USKInventoryObjectData *> InventoryData;

    USKInventoryObjectData *CreateInventoryObjectDataItem(const AActor *Item);
    USKInventoryObjectData *SplitInventoryObjectData(USKInventoryObjectData *ObjectData, const uint32 SplitAmount);
    USKInventoryObjectData * FindInventoryItem(USKInventoryObjectData * ObjectData);
    void SortInventory();

    UPROPERTY()
    TObjectPtr<USKInventoryObjectData> MainWeaponSlot = nullptr;
};