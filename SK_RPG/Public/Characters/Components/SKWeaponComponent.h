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
    UFUNCTION(BlueprintCallable, Category = "AA SK Weapon Component")
    void SpawnWeapon(TSubclassOf<ASKEquippableBase> EquippableClass);

    UFUNCTION(BlueprintCallable, Category = "SK Weapon Component")
    void DestroyWeapon();

    UFUNCTION(BlueprintCallable, Category = "SK Weapon Component")
    void SetIsTracingSword(bool Value);

    UFUNCTION(BlueprintPure, Category = "SK Weapon Component")
    ASKEquippableBase *GetEquippedWeapon() { return EquippedWeapon.Get(); }

    UFUNCTION(BlueprintCallable, Category = "SK Weapon Component")
    void TogglePhysics();

  protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

  private:
    UPROPERTY()
    TObjectPtr<ASKEquippableBase> EquippedWeapon = nullptr;

    bool bIsPhysicsActive = false;
    bool bIsTracingSword = false;
};
