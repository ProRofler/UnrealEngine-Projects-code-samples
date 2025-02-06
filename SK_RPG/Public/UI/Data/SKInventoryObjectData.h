// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Core/SKCoreTypes.h"
#include "CoreMinimal.h"
#include "Gameplay/Interactables/SKCollectible.h"
#include "UObject/NoExportTypes.h"

#include "SKInventoryObjectData.generated.h"

UCLASS()
class SIRKNIGHT_API USKInventoryObjectData : public UObject
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable)
    const FName &GetItemName() const { return ItemName; }
    const int32 &GetItemQuantity() const { return ItemQuantity; }
    const TSubclassOf<ASKCollectible> GetItemClass() const { return ItemClass; }

    void IncreaseItemQuantity(const int32 QuantityToIncrease) { ItemQuantity += QuantityToIncrease; }
    void DecreaseItemQuantity(const int32 QuantityToDecreases) { ItemQuantity -= QuantityToDecreases; }

    void InitializeItemData(const FName &_ItemName, const int32 _ItemQuantity,
                            const TSubclassOf<ASKCollectible> _ItemClass);

  private:
    FName ItemName = TEXT("Default name");
    int32 ItemQuantity = 0;
    TSubclassOf<ASKCollectible> ItemClass;
};
