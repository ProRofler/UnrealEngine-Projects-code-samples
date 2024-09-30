// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/S_RifleWeapon.h"
#include "Dev/S_TestDamageType.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Weapons/S_FXWeaponComponent.h"
#include "Weapons/S_PlayerWeaponComponent.h"

void AS_RifleWeapon::Fire()
{

    // UE_LOG(LogTemp, Display, TEXT("FIRE! triggered"));
    Shot();
    InitMuzzleFX();
    GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AS_RifleWeapon::Shot, FireRate, true);
}

void AS_RifleWeapon::StopFire()
{
    // UE_LOG(LogTemp, Display, TEXT("FIRE! stopped"));
    GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
    SetMuzzleFXVisibility(false);
}

void AS_RifleWeapon::Shot()
{
    if (!GetWorld())
        return;

    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return;

    const auto Controller = Player->GetController();
    if (!Controller)
        return;

    if (IsClipEmpty())
    {
        // Reload();
        Player->GetComponentByClass<US_PlayerWeaponComponent>()->Reload();
        StopFire();
        return;
    }

    if (IsAmmoEmpty() && IsClipEmpty())
    {
        //UE_LOG(LogTemp, Display, TEXT("---NO AMMO---"));
        StopFire();
        return;
    }

    FVector PlayerLocation;
    FRotator PlayerRotation;

    const auto CharCheck = Cast<ACharacter>(GetOwner());
    if (!CharCheck)
        return;

    if (CharCheck->IsPlayerControlled())
    {
        Controller->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
    }
    else
    {
        PlayerLocation = WeaponMeshComponent->GetSocketLocation(MuzzleSocket);
        PlayerRotation = WeaponMeshComponent->GetSocketRotation(MuzzleSocket);

    }

    FVector RandomDirection = FVector(FMath::RandRange(-0.01f, 0.01f), 0, FMath::RandRange(-0.01f, 0.01f));
    /*RandomDirection.X = FMath::Clamp(FMath::SRand(), 0.0005f, 0.05f);
    RandomDirection.Y = FMath::Clamp(FMath::SRand(), 0.0005f, 0.05f);*/

    FTransform SocketTransform = WeaponMeshComponent->GetSocketTransform(MuzzleSocket);
    if (!SocketTransform.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Can't find the muzzle socket"));
        checkNoEntry();
    }

    FVector TraceStart = SocketTransform.GetLocation();
    FVector TraceDirection =
        PlayerRotation.Vector() + RandomDirection; // SocketTransform.GetRotation().GetForwardVector();
    FVector TraceEnd = TraceStart + TraceDirection * TraceDistance;

    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(GetOwner());
    TraceParams.bReturnPhysicalMaterial = true;

    // FHitResult HitFromCamera = TraceFromCamera();

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceFromCamera().ImpactPoint,
                                         ECollisionChannel::ECC_Visibility, TraceParams);
    DecreaseAmmo();

    FVector TraceFXEnd;

    if (TraceFromCamera().bBlockingHit)
    {
        // TraceFXEnd = TraceFromCamera().ImpactPoint;
        // DrawDebugLine(GetWorld(), TraceStart, TraceFromCamera().ImpactPoint, FColor::Red, false, 4.0f, 0, 0.1f);
        TraceFXEnd = TraceFromCamera().ImpactPoint;
        // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 12.0f, 16, FColor::Green, false, 4.0f, 0, 0.5f);
        //  UE_LOG(LogTemp, Display, TEXT("Hit bone: %s"), *HitResult.BoneName.ToString());
        FXComponent->SpawnEmitter(HitResult);

        if (HitResult.GetActor())
        {
            const auto DamagedActor = HitResult.GetActor();
            const float Damage = FMath::Clamp(FMath::SRand() * MaxDamage, 8.0f, MaxDamage);
            UGameplayStatics::ApplyPointDamage(DamagedActor, Damage, HitResult.ImpactNormal, HitResult,
                                               DamagedActor->GetInstigatorController(), GetOwner(), DamageType);
        }
    }
    else
    {
        //UE_LOG(LogTemp, Display, TEXT("No hit"));
        // DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 4.0f, 0, 0.1f);
        TraceFXEnd = TraceEnd;
    }

    SpawnTraceFX(TraceStart, TraceFXEnd);
}

void AS_RifleWeapon::InitMuzzleFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnNiagaraFX();
    }
    SetMuzzleFXVisibility(true);
}

void AS_RifleWeapon::SetMuzzleFXVisibility(bool Visibility)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!Visibility);
        MuzzleFXComponent->SetVisibility(Visibility);
    }
}

void AS_RifleWeapon::SpawnTraceFX(const FVector &TraceStart, const FVector &TraceEnd)
{
    const auto TraceFXComponent =
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FXComponent->TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(FXComponent->TraceTargetName, TraceEnd);
    }
}
