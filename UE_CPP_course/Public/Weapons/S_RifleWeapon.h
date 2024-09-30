// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/S_BaseWeapon.h"
#include "S_RifleWeapon.generated.h"

class UNiagaraComponent;

UCLASS()
class CPP_02_API AS_RifleWeapon : public AS_BaseWeapon
{
    GENERATED_BODY()

  public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float TraceDistance = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float SpreadAmount = 0.1f;

    virtual void Fire() override;
    virtual void StopFire() override;

  protected:

      UPROPERTY()
      UNiagaraComponent * MuzzleFXComponent;

  private:
    virtual void Shot() override;
    FTimerHandle FireTimerHandle;

    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool Visibility);
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

};
