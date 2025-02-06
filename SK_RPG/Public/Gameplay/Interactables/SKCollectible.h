// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Core/Interface/SKInterfaceCollectible.h"
#include "CoreMinimal.h"
#include "Gameplay/Interactables/SKInteractableBase.h"

#include "SKCollectible.generated.h"

UCLASS()
class SIRKNIGHT_API ASKCollectible : public ASKInteractableBase, public ISKInterfaceCollectible
{
    GENERATED_BODY()

  public:
    ASKCollectible();

    UFUNCTION(BlueprintCallable)
    const int32 &GetItemQuantity() const { return Quantity; }

    virtual void OnInteraction_Implementation(const AActor *TriggeredActor) override;

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Collectibles")
    int32 Quantity = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Collectibles",
              meta = (ClampMin = "0.0001", UIMin = "0.0"))
    float Weight = 1.0f;
};
