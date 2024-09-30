// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/S_PlayerWeaponComponent.h"
#include "S_AIWeaponComponent.generated.h"



UCLASS()
class CPP_02_API US_AIWeaponComponent : public US_PlayerWeaponComponent
{
    GENERATED_BODY()

  public:
    virtual void Fire() override;
};
