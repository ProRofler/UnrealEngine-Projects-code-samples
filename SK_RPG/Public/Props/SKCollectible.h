// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Props/SKInteractableBase.h"

#include "SKCollectible.generated.h"

UCLASS()
class SIRKNIGHT_API ASKCollectible : public ASKInteractableBase
{
    GENERATED_BODY()

  public:
    ASKCollectible();
    bool bIsInInventory = false;

    UFUNCTION(BlueprintCallable)
    int32 GetItemQuantity() const { return Quantity; }

    virtual void OnInteraction_Implementation(const AActor *TriggeredActor) override;

  protected:
  private:
    int32 Quantity;

    void HideFromWorld();
    void ShowInWorld();
};
