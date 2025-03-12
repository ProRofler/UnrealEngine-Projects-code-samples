// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/Interface/SKInterfaceWeapon.h"

#include "Gameplay/Interactables/SKCollectible.h"

#include "Gameplay/Equipables/SKEquippableBase.h"

#include "SKWeapon.generated.h"

UCLASS()
class SIRKNIGHT_API ASKWeapon : public ASKCollectible, public ISKInterfaceWeapon
{
    GENERATED_BODY()

  public:
    ASKWeapon();

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Weapon settings")
    TSubclassOf<ASKEquippableBase> EquippableWeaponClass;

    virtual void OnConstruction(const FTransform &Transform);

  private:
    TSubclassOf<ASKEquippableBase> TSubclaGetEquippableWeaponClass_Implementation() { return EquippableWeaponClass; }
};
