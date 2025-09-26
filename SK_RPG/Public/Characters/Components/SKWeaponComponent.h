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
    void SpawnWeapon();

    UFUNCTION(BlueprintCallable, Category = "SK Weapon Component")
    void DestroyWeapon();

    UFUNCTION(BlueprintCallable, Category = "SK Weapon Component")
    void SetIsTracingMelee(bool Value);

    UFUNCTION(BlueprintPure, Category = "SK Weapon Component")
    ASKEquippableBase *GetEquippedWeapon() { return EquippedWeapon.Get(); }

    /*
    this method will switch between simulated physics and regular collision for the main weapon in hands
    attachment and grab must be set up in character itself
    */
    UFUNCTION(BlueprintCallable, Category = "SK Weapon Component")
    void SetMainWeaponMeshPhysics(const bool IsActive);

    /*
    this method is used to softly reset the position of a weapon in hands back to zero
    it's needed after physical hitstop cause the position of grabbed weapon most likely will be different from
    attached
    */
    UFUNCTION(BlueprintCallable, Category = "SK Weapon Component")
    void SoftResetWeaponTransform(const float Speed);

  protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

    // TEMP SHIT!!!!! FIX ASAP
    UPROPERTY(EditDefaultsOnly, Category = "SK Weapon FX")
    UMaterialInterface *DecalMat = nullptr;

  private:
    UPROPERTY()
    TObjectPtr<ASKEquippableBase> EquippedWeapon = nullptr;

    void HandleWeaponTrace() const;

    bool bIsTracingSword = false;
};
