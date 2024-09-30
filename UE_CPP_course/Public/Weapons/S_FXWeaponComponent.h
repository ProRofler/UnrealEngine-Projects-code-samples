// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "S_FXWeaponComponent.generated.h"

class UNiagaraSystem;
class UPhysicalMaterial;

USTRUCT()
struct FDecalData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Weapon FX")
    UMaterialInterface *Decal = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon FX")
    FVector Size = FVector(10.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Weapon FX")
    float LifeTime = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon FX")
    float FadeOutTime = 1.0f;
};

USTRUCT(BlueprintType)
struct FImpactData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Weapon FX")
    UNiagaraSystem *ImpactFX = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon FX")
    FDecalData DecalData;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPP_02_API US_FXWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    US_FXWeaponComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

    void SpawnEmitter(const FHitResult &HitResult) const;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon FX")
    UNiagaraSystem *MuzzleFlashFX = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon FX")
    UNiagaraSystem *TraceFX = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon FX")
    FString TraceTargetName = "BeamEnd";

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon FX")
    FImpactData DefaultImpactFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon FX")
    TMap<UPhysicalMaterial *, FImpactData> FXMap;
};
