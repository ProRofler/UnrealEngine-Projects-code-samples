// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Weapons/S_BaseWeapon.h"
#include "S_PlayerWidget.generated.h"


UCLASS()
class CPP_02_API US_PlayerWidget : public UUserWidget
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintPure, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintPure, Category = "UI")
    bool GetWeaponUIData(FUIData &UIData) const;

    UFUNCTION(BlueprintPure, Category = "UI")
    bool GetWeaponAmmoData(FAmmoData &AmmoData) const;

    UFUNCTION(BlueprintPure, Category = "UI")
    bool isPlayerSpectating() const;

    virtual bool Initialize() override;

    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChanged();

  private:
    void OnTakeDamage(float Health, float HealthDelta);
};
