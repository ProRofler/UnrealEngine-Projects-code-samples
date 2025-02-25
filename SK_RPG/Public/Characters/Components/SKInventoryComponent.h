// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SKInventoryComponent.generated.h"

class USKInventoryObjectData;
class ASKBaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickupDelegateSignature, const AActor *, PickedItem);
DECLARE_DYNAMIC_DELEGATE(FOnInventoryUpdatedSignature);

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

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool IsInInventory(const FName &Name) const; // FName version

    UPROPERTY(BlueprintAssignable, Category = "Interactions")
    FOnItemPickupDelegateSignature OnItemPickup;
    UPROPERTY(BlueprintReadOnly, Category = "Interactions")
    FOnInventoryUpdatedSignature OnInventoryUpdated;

    TArray<TObjectPtr<USKInventoryObjectData>> &GetInventoryData() { return InventoryData; }

  protected:
    virtual void BeginPlay() override;

  private:
    ASKBaseCharacter *OwningCharacter = nullptr;

    UPROPERTY()
    TArray<TObjectPtr<USKInventoryObjectData>> InventoryData;

    TObjectPtr<USKInventoryObjectData> CreateInventoryObjectDataItem(const AActor *Item);

    void SortInventory();

    ASKBaseCharacter *GetOwningCharacter();
    void InitDelegates();

    USKInventoryObjectData *FindInventoryItem(const USKInventoryObjectData *ObjectData);
};