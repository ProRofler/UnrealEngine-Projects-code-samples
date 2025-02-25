// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Interactables/SKCollectible.h"
#include "SKKeyItem.generated.h"

UCLASS()
class SIRKNIGHT_API ASKKeyItem : public ASKCollectible
{
    GENERATED_BODY()

  public:
    UPROPERTY(EditInstanceOnly, Category = "SK Lock Settings")
    FString KeyID = TEXT("None");

  protected:
    virtual void BeginPlay() override;
};
