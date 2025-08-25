// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKWeaponComponent.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "UI/Data/SKInventoryObjectData.h"

#include "Gameplay/Equipables/SKEquippableSword.h"
#include "Gameplay/Interactables/SKWeapon.h"

#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/SKBaseCharacter.h"

#include "Gameplay/GAS/SKNativeGameplayTags.h"

void USKWeaponComponent::BeginPlay() { Super::BeginPlay(); }

void USKWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsTracingSword)
    {
        FHitResult HitResult;
        Cast<ASKEquippableSword>(EquippedWeapon)->HitDetect(HitResult);

        if (HitResult.bBlockingHit)
        {
            FGameplayEventData Payload;
            // TODO: Send hit info
            Payload.EventTag = FSKGameplayTags::Get().Event_Combat_Hit;
            Payload.Instigator = GetOwner();
            Payload.Target = HitResult.GetActor();

            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetSKOwnerCharacter(),
                                                                     FSKGameplayTags::Get().Event_Combat_Hit, Payload);
        }
    }
}

void USKWeaponComponent::SpawnWeapon(TSubclassOf<ASKEquippableBase> EquippableClass)
{

    if (!EquippableClass) return;

    EquippedWeapon =
        GetWorld()->SpawnActorDeferred<ASKEquippableSword>(EquippableClass, GetOwner()->GetActorTransform());

    if (!EquippedWeapon) checkNoEntry();

    EquippedWeapon->SetActorEnableCollision(false);

    EquippedWeapon->AttachToComponent(GetSKOwnerCharacter()->GetMesh(),
                                      FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");

    EquippedWeapon->FinishSpawning(GetOwner()->GetActorTransform());
}

void USKWeaponComponent::DestroyWeapon()
{
    if (!EquippedWeapon) return;

    EquippedWeapon->Destroy();
}

void USKWeaponComponent::SetIsTracingMelee(bool Value)
{

    switch (Value)
    {
    case 1: bIsTracingSword = true; break;

    default:
        bIsTracingSword = false;
        Cast<ASKEquippableSword>(EquippedWeapon)->ResetTraceStartEnd();
        break;
    }
}

void USKWeaponComponent::SetSkeletalMeshPhysics(const bool IsActive)
{

    const auto weaponMeshComponent = EquippedWeapon->FindComponentByClass<UStaticMeshComponent>();
    if (!weaponMeshComponent) return;

    EquippedWeapon->SetActorEnableCollision(IsActive);
    weaponMeshComponent->SetSimulatePhysics(IsActive);

    if (IsActive)
    {
        weaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
        weaponMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
        weaponMeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
    }
    else
    {
        weaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        weaponMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
        weaponMeshComponent->SetCollisionObjectType(ECC_Visibility);

        EquippedWeapon->AttachToComponent(GetSKOwnerCharacter()->GetMesh(),
                                          FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");
    }
}