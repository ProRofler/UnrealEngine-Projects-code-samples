// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SKInventoryComponent.generated.h"

class ASKCollectible;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIRKNIGHT_API USKInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USKInventoryComponent();

    void AddToInventory(const TObjectPtr<AActor> Item);
    void RemoveFromInventory(const TObjectPtr<AActor> Item);
    void SortInventory();

    TArray<TObjectPtr<AActor>> GetInventoryData() const { return InventoryData; }

    UFUNCTION(BlueprintPure)
    int32 GetInventorySize() { return InventoryData.Num(); }

    UFUNCTION(BlueprintPure)
    AActor *GetItemByIndex(int32 Index = 0)
    {
        //UE_LOG(LogTemp, Display, TEXT("CHECKINGGGGG"));
        return InventoryData.IsValidIndex(Index) ? InventoryData[Index].Get() : nullptr;
    }

  protected:
    virtual void BeginPlay() override;

  private:
    TArray<TObjectPtr<AActor>> InventoryData;
};