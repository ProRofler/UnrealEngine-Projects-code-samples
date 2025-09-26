
#pragma once

#include "CoreMinimal.h"

#include "S_BaseWeapon.generated.h"

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 ClipSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 AmmoCurrent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo", meta = (EditCondition = "!bInfiniteAmmo"))
    int32 AmmoTotal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo", meta = (EditCondition = "!bInfiniteAmmo"))
    int32 AmmoMax;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    bool bInfiniteAmmo;
};

USTRUCT(BlueprintType)
struct FUIData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UTexture2D *Crosshair;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UTexture2D *WeaponIcon;
};

class US_FXWeaponComponent;
class UNiagaraComponent;

UCLASS()
class CPP_02_API AS_BaseWeapon : public AActor
{
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    AS_BaseWeapon();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon mesh")
    USkeletalMeshComponent *WeaponMeshComponent = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    FName MuzzleSocket = "Muzzle";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shooting")
    US_FXWeaponComponent * FXComponent;

    virtual void Fire();
    virtual void StopFire();
    void Reload();
    void AddAmmo(int32 AmmoAmount);
    bool IsClipFull() const{return CurrentAmmo.AmmoCurrent == CurrentAmmo.ClipSize;};
    bool IsClipEmpty() const{return CurrentAmmo.AmmoCurrent == 0;};
    bool IsAmmoEmpty(){return CurrentAmmo.AmmoTotal == 0;};
    FUIData GetWeaponUIData() const{return WeaponUIData;};
    FAmmoData AmmoLog(){return CurrentAmmo;};

    // functions

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    TSubclassOf<UDamageType> DamageType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float MaxDamage = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float FireRate = 4.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon settings")
    FAmmoData AmmoSettings{30, 30, 60, false};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Settings")
    FUIData WeaponUIData;

    UNiagaraComponent * SpawnNiagaraFX();

    FHitResult TraceFromCamera();

    void DecreaseAmmo();

  private:
    virtual void Shot();
    FAmmoData CurrentAmmo;
};
