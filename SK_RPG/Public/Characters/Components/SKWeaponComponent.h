// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Components/SKCharacterComponentBase.h"

#include "Gameplay/Equipables/SKEquippableBase.h"

#include "SKWeaponComponent.generated.h"

class USKInventoryObjectData;
class ASKWeapon;

UCLASS()
class SIRKNIGHT_API USKWeaponComponent : public USKCharacterComponentBase
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = "SK Weapon Component")
    void SpawnWeapon(TSubclassOf<ASKEquippableBase> EquippableClass);

    UFUNCTION(BlueprintCallable, Category = "SK Weapon Component")
    void DestroyWeapon();

  protected:
    virtual void BeginPlay() override;

  private:
    UPROPERTY()
    TObjectPtr<ASKEquippableBase> EquippedWeapon = nullptr;
};
