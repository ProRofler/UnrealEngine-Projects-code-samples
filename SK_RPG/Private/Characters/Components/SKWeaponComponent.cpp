// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKWeaponComponent.h"

#include "UI/Data/SKInventoryObjectData.h"

#include "Gameplay/Interactables/SKWeapon.h"

#include "Gameplay/Equipables/SKEquippableSword.h"

#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/SKBaseCharacter.h"

void USKWeaponComponent::BeginPlay() { Super::BeginPlay(); }

void USKWeaponComponent::SpawnWeapon(TSubclassOf<ASKEquippableBase> EquippableClass)
{

    if (!EquippableClass) return;

    EquippedWeapon =
        GetWorld()->SpawnActorDeferred<ASKEquippableSword>(EquippableClass, GetOwner()->GetActorTransform());

    if (!EquippedWeapon) checkNoEntry();

    EquippedWeapon->SetActorEnableCollision(false);

    EquippedWeapon->AttachToComponent(GetSKOwnerCharacter()->GetMesh(),
                                      FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                                      "RightHandWeaponSocket");

    EquippedWeapon->FinishSpawning(GetOwner()->GetActorTransform());
}

void USKWeaponComponent::DestroyWeapon()
{
    if (!EquippedWeapon) return;

    EquippedWeapon->Destroy();
}