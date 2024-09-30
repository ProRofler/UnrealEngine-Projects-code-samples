// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/S_AIWeaponComponent.h"


void US_AIWeaponComponent::Fire()
{
    if (!PrimaryWeapon || !SecondaryWeapon)
    {
        return;
    }
    else if (IsAmmoEmpty())
    {
        return;
    }
    else if (IsPrimaryAmmoEmpty() && CanSwitch())
    {
        SwitchWeapon();
        return;
    }
    else
    {
        US_PlayerWeaponComponent::Fire();
    }
}
