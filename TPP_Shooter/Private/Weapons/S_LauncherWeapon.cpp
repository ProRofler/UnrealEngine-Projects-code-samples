// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/S_LauncherWeapon.h"
// #include "Kismet/GameplayStatics.h"
#include "S_Projectile.h"
#include "Weapons/S_PlayerWeaponComponent.h"
#include "GameFramework/Character.h"

void AS_LauncherWeapon::Fire()
{
    if (IsAmmoEmpty() && IsClipEmpty())
    {
        UE_LOG(LogTemp, Display, TEXT("---NO AMMO---"));
        return;
    }
    // UE_LOG(LogTemp, Display, TEXT("FIRE! triggered"));
    Shot();
    SpawnNiagaraFX();
}

void AS_LauncherWeapon::Shot()
{
    if (!GetWorld())
        return;
    // set params for projectile
    // UGameplayStatics::FinishSpawningActor(Projectile, WeaponMeshComponent->GetSocketTransform(MuzzleSocket)); // old
    // method usint kismet

    // because I didn't refactored the code for shooting, it's gonna be copied here

    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return;

    const auto Controller = Player->GetController();
    check(Controller)

    if (IsClipEmpty())
    {
        //Reload();
        Player->GetComponentByClass<US_PlayerWeaponComponent>()->Reload();
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

    FTransform SocketTransform = WeaponMeshComponent->GetSocketTransform(MuzzleSocket);
    FVector TraceStart = SocketTransform.GetLocation();
    FVector TraceDirection = PlayerRotation.Vector(); // SocketTransform.GetRotation().GetForwardVector();
    FVector TraceEnd = TraceStart + TraceDirection * 2000.0f;

    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility,
                                         TraceParams);
    DecreaseAmmo();

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - TraceStart).GetSafeNormal();

    const auto Projectile = GetWorld()->SpawnActorDeferred<AS_Projectile>(
        ProjectileComponent, WeaponMeshComponent->GetSocketTransform(MuzzleSocket));

    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(WeaponMeshComponent->GetSocketTransform(MuzzleSocket));
        Projectile->SetLifeSpan(5.0f);
    }
}
