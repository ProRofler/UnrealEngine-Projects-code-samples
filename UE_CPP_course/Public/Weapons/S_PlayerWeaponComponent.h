// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "S_PlayerWeaponComponent.generated.h"

// Forward declarations
class AS_BaseWeapon;
struct FUIData;
struct FAmmoData;

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon choice")
    TSubclassOf<AS_BaseWeapon> PrimaryWeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon choice")
    TSubclassOf<AS_BaseWeapon> SecondaryWeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Reload animation")
    UAnimMontage *PrimaryRealoadAnimation = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Reload animation")
    UAnimMontage *SecondaryRealoadAnimation = nullptr;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPP_02_API US_PlayerWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    US_PlayerWeaponComponent();
    void SpawnWeapons();
    virtual void Fire();
    void StopFire();
    void SwitchWeapon();
    void Reload();
    void DetachMainWeapon();

    bool CanShoot(){return PrimaryWeapon && bCanShoot;};
    bool CanSwitch(){return bCanShoot;};
    bool IsPrimaryAmmoEmpty();
    bool IsSecondaryAmmoEmpty();
    bool IsAmmoEmpty();
    bool GetWeaponUIData(FUIData &UIData);
    bool GetWeaponAmmoData(FAmmoData &AmmoData);
    FWeaponData GetWeaponData();

    UPROPERTY()
    AS_BaseWeapon *PrimaryWeapon;
    UPROPERTY()
    AS_BaseWeapon *SecondaryWeapon;

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName PrimaryWeaponSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName SecondarySocketName = "SecondWeaponSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimMontage *WeaponSwitchAnimation = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FWeaponData WeaponData;

    bool bCanShoot = true;

  private:
    UPROPERTY()
    UAnimMontage *PrimaryReloadAnimation = nullptr;
    UPROPERTY()
    UAnimMontage *SecondaryReloadAnimation = nullptr;
    UPROPERTY()
    UAnimMontage *CurrentReloadAnimation = nullptr;

    void AttachWeapons();
    void PlayAnimationMontage(UAnimMontage *Animation);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent *MeshComp);

};
