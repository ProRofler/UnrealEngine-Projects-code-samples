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

  public:
    USKInventoryComponent();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddToInventory(AActor *PickedItem);
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RemoveFromInventory(USKInventoryObjectData *ItemToRemove, const int32 QuantityToDrop);
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool IsInventoryEmpty() const { return InventoryData.IsEmpty(); }

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool IsInInventoryByClass(const TSubclassOf<ASKCollectible> &CollectibleClass) const; // Collectible class overload

    UPROPERTY(BlueprintAssignable, Category = "Interactions")
    FOnItemPickupDelegateSignature OnItemPickup;
    UPROPERTY(BlueprintReadOnly, Category = "Interactions")
    FOnInventoryUpdatedSignature OnInventoryUpdated;

    TArray<TObjectPtr<USKInventoryObjectData>> &GetInventoryData() { return InventoryData; }

  protected:
    virtual void BeginPlay() override;

  private:

    UPROPERTY()
    TArray<TObjectPtr<USKInventoryObjectData>> InventoryData;

    TObjectPtr<USKInventoryObjectData> CreateInventoryObjectDataItem(const AActor *Item);
    USKInventoryObjectData *FindInventoryItem(const USKInventoryObjectData *ObjectData);
    void SortInventory();

    void InitDelegates();

};