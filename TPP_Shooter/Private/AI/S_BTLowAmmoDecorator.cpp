// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/S_BTLowAmmoDecorator.h"
#include "AIController.h"
#include "AI/S_AIWeaponComponent.h"
#include "S_BaseWeapon.h"
#include "Pickups/S_PickupAmmo.h"

US_BTLowAmmoDecorator::US_BTLowAmmoDecorator()
{
    NodeName = "Ammo decorator";
}

bool US_BTLowAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
        return false;
    // UE_LOG(LogTemp, Display, TEXT("GOT controller"));
    const auto WeaponComponent = Controller->GetPawn()->GetComponentByClass<US_AIWeaponComponent>();
    if (!WeaponComponent)
        return false;
    // UE_LOG(LogTemp, Display, TEXT("GOT component"));

    //UE_LOG(LogTemp, Display, TEXT("AMMO SEARCHING!!!!"));
    return (WeaponComponent->GetWeaponData().PrimaryWeaponClass == WeaponAmmoType && WeaponComponent->PrimaryWeapon->IsAmmoEmpty()) || (WeaponComponent->GetWeaponData().SecondaryWeaponClass && WeaponComponent->SecondaryWeapon->IsAmmoEmpty());

}
