// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/S_BaseWeapon.h"
#include "S_LauncherWeapon.generated.h"


class AS_Projectile;

UCLASS()
class CPP_02_API AS_LauncherWeapon : public AS_BaseWeapon
{
    GENERATED_BODY()

  public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    TSubclassOf<AS_Projectile> ProjectileComponent = nullptr;

    virtual void Fire() override;

  protected:
  private:
    virtual void Shot() override;
};
