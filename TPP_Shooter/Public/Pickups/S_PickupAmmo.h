// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/S_PickupBase.h"
#include "S_PickupAmmo.generated.h"

class AS_BaseWeapon;

UCLASS()
class CPP_02_API AMyS_PickupAmmo : public AS_PickupBase
{
    GENERATED_BODY()

    TSubclassOf<AS_BaseWeapon> GetWeaponType(){return WeaponType;};
  protected:
    //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup settings");
    TSubclassOf<AS_BaseWeapon> WeaponType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup settings");
    int32 AmmoAmount = 15;

  private:
    virtual void OnPickup(AActor *PlayerActor) override;
};
