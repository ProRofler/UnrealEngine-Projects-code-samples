// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/Interface/SKInterfaceCollectible.h"
#include "Core/SKCoreTypes.h"

#include "Gameplay/Interactables/SKInteractableBase.h"

#include "SKCollectible.generated.h"

UCLASS()
class SIRKNIGHT_API ASKCollectible : public ASKInteractableBase, public ISKInterfaceCollectible
{
    GENERATED_BODY()

  public:
    ASKCollectible();

    UFUNCTION(BlueprintPure)
    const FORCEINLINE int32 &GetItemQuantity() const { return Quantity; }

    FORCEINLINE void SetItemQuantity(const int32 ItemQuantity) { Quantity = ItemQuantity; }

    UFUNCTION(BlueprintPure)
    const FORCEINLINE ECollectibleType GetCollectibleType() const { return CollectibleType; }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE void SetCollectibleType(const ECollectibleType _CollectibleType) { CollectibleType = _CollectibleType; }

    virtual void OnInteraction_Implementation(const AActor *TriggeredActor) override;

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Collectibles")
    int32 Quantity = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Collectibles",
              meta = (ClampMin = "0.0001", UIMin = "0.0"))
    float Weight = 1.0f;

  private:
    ECollectibleType CollectibleType = ECollectibleType::Misc;
};
