// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class CPP_02_API AS_Projectile : public AActor
{
    GENERATED_BODY()

  public: 
    AS_Projectile();

    USphereComponent *CollisionComponent = nullptr;

    UPROPERTY(BlueprintReadWrite, Category = "Projectile settings")
    UProjectileMovementComponent *ProjectileMovementComponent = nullptr;

    void SetShotDirection(const FVector &Direction)
    {
        ProjectileDirection = Direction;
    };

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile settings")
    float DamageRadius = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile settings")
    float DamageAmount = 85.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shooting FX")
    US_FXWeaponComponent *FXComponent;

  private:
    FVector ProjectileDirection;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                         FVector NormalImpulse, const FHitResult &Hit);

    AActor *Owner = nullptr;

    AController *GetController();
};
