// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKWeapon.h"

#include "Gameplay/Equipables/SKEquippableBase.h"

ASKWeapon::ASKWeapon() { bCanEditMesh = false; }

void ASKWeapon::OnConstruction(const FTransform &Transform)
{
    if (EquippableWeaponClass)
    {
        const auto eqippableBase = Cast<ASKEquippableBase>((EquippableWeaponClass->GetDefaultObject()));
        if (!eqippableBase) return;

        BaseMesh->SetStaticMesh(eqippableBase->GetStaticMesh());
    }
}
