// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/S_PickupBase.h"
#include "S_PickupHealth.generated.h"

/**
 *
 */
UCLASS()
class CPP_02_API AMyS_PickupHealth : public AS_PickupBase
{
    GENERATED_BODY()

  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup settings")
    float HealthAmount = 15.0f;

  private:
    virtual void OnPickup(AActor *PlayerActor) override;
};
