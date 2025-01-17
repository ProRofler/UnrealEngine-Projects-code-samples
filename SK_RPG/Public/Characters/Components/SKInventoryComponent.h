// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SKInventoryComponent.generated.h"

class USKInventoryObjectData;
class ASKBaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickupDelegate, const AActor *, PickedItem);
DECLARE_DYNAMIC_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIRKNIGHT_API USKInventoryComponent : public UActorComponent
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

    UPROPERTY(BlueprintAssignable, Category = "Interactions")
    FOnItemPickupDelegate OnItemPickup;
    UPROPERTY(BlueprintReadOnly, Category = "Interactions")
    FOnInventoryUpdated OnInventoryUpdated;

    TArray<TObjectPtr<USKInventoryObjectData>> &GetInventoryData() { return InventoryData; }

  protected:
    virtual void BeginPlay() override;

  private:
    TArray<TObjectPtr<USKInventoryObjectData>> InventoryData;
    ASKBaseCharacter *OwningCharacter = nullptr;

    TObjectPtr<USKInventoryObjectData> CreateInventoryObjectDataItem(const AActor *Item);

    void SortInventory();

    ASKBaseCharacter *GetOwningCharacter();
    void InitDelegates();

    TObjectPtr<USKInventoryObjectData> FindInventoryItem(const USKInventoryObjectData *ObjectData) const;
};