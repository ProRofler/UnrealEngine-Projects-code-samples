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
    const FORCEINLINE FName &GetItemName() const { return ItemName; }
    const FORCEINLINE int32 &GetItemQuantity() const { return ItemQuantity; }
    const FORCEINLINE TSubclassOf<ASKCollectible> GetItemClass() const { return ItemClass; }
    const FORCEINLINE ECollectibleType GetItemType() const { return ItemType; }

    void ChangeItemQuantity(const int32 QuantityToChange) { ItemQuantity += QuantityToChange; }

    void InitializeItemData(const FName &_ItemName, const int32 _ItemQuantity,
                            const TSubclassOf<ASKCollectible> _ItemClass, const ECollectibleType _ItemType);

  private:
    FName ItemName = TEXT("Default name");
    ECollectibleType ItemType;
    int32 ItemQuantity = 0;
    TSubclassOf<ASKCollectible> ItemClass;
};

UCLASS()
class SIRKNIGHT_API USKInventoryObjectDataKey : public USKInventoryObjectData
{
    GENERATED_BODY()

  public:
    void AddKeyID(const FGuid &_KeyID) { KeyID = _KeyID; }
    const FORCEINLINE FGuid GetKeyID() const { return KeyID; }

  private:
    FGuid KeyID;
};