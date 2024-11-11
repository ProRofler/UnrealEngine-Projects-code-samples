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
    void RemoveFromInventory(AActor *ItemToRemove);
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool IsInventoryEmpty() const { return InventoryData.IsEmpty(); }

    UPROPERTY(BlueprintAssignable, Category = "Interactions")
    FOnItemPickupDelegate OnItemPickup;
    UPROPERTY(BlueprintReadOnly, Category = "Interactions")
    FOnInventoryUpdated OnInventoryUpdated;

    TArray<TWeakObjectPtr<AActor>> &GetInventoryData() { return InventoryData; }

    UFUNCTION(BlueprintPure)
    int32 GetInventorySize() { return InventoryData.Num(); }

  protected:
    virtual void BeginPlay() override;

  private:
    TArray<TWeakObjectPtr<AActor>> InventoryData;
    ASKBaseCharacter *OwningCharacter = nullptr;

    void SortInventory();

    ASKBaseCharacter *GetOwningCharacter();
    void InitDelegates();
};