// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/S_PickupAmmo.h"
#include "Weapons/S_PlayerWeaponComponent.h"
#include "Weapons/S_BaseWeapon.h"

void AMyS_PickupAmmo::OnPickup(AActor* PlayerActor)
{
    //UE_LOG(LogTemp, Display, TEXT("Picked up - %s by %s"), *GetActorNameOrLabel(), *PlayerActor->GetActorNameOrLabel());
    
    const auto WeaponComponent = PlayerActor->GetComponentByClass<US_PlayerWeaponComponent>();
    if(!WeaponComponent) return;
    const auto WeaponData = Cast<AS_BaseWeapon>(WeaponComponent->PrimaryWeapon);

    if (WeaponData && WeaponData->IsA(WeaponType))
    {
        //UE_LOG(LogTemp, Display, TEXT("Primary weapon is the same as pickup"));
        WeaponComponent->PrimaryWeapon->AddAmmo(AmmoAmount);
    }
    else
    {
        //UE_LOG(LogTemp, Display, TEXT("Secondary weapon is the same as pickup"));
         WeaponComponent->SecondaryWeapon->AddAmmo(AmmoAmount);
    }
}
