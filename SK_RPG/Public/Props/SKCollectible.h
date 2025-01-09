// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Core/Interface/SKInterfaceCollectible.h"
#include "CoreMinimal.h"
#include "Props/SKInteractableBase.h"

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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base properties | Collectibles")
    int32 Quantity = 1;
};
