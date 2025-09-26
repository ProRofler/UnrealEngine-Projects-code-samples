// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKWeaponComponent.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "UI/Data/SKInventoryObjectData.h"

#include "Gameplay/Equipables/SKEquippableSword.h"
#include "Gameplay/Interactables/SKWeapon.h"

#include "Characters/Components/SKInventoryComponent.h"
#include "Core/Interface/SKInterfaceCharacter.h"
#include "Characters/SKBaseCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

#include "Gameplay/GAS/SKNativeGameplayTags.h"

void USKWeaponComponent::BeginPlay() { Super::BeginPlay(); }

void USKWeaponComponent::HandleWeaponTrace() const
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

        // TEMP DECAL SPAWN
        if (DecalMat)
        {
            const auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),                         //
                                                                      DecalMat,                           //
                                                                      FVector(10.f),                      //
                                                                      HitResult.ImpactPoint,              //
                                                                      HitResult.ImpactNormal.Rotation()); //
            if (Decal) Decal->SetFadeOut(1.f, 2.f);
        }
    }
}

void USKWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsTracingSword && EquippedWeapon) HandleWeaponTrace();
}

void USKWeaponComponent::SpawnWeapon()
{

    USKInventoryComponent *inventory = ISKInterfaceCharacter::Execute_GetInventoryComponent(GetOwner());
    if (!inventory) return;

    USKInventoryObjectData *mainWeaponSlot = inventory->GetMainWeaponSlot();
    if (!mainWeaponSlot) return;

    const auto weaponCDO = Cast<ASKWeapon>(mainWeaponSlot->GetItemClass()->GetDefaultObject());
    if (!weaponCDO || !weaponCDO->GetMesh() || !weaponCDO->GetMesh()->GetStaticMesh()) return;

    auto spawnedWeapon = GetWorld()->SpawnActorDeferred<ASKEquippableSword>(ASKEquippableSword::StaticClass(),
                                                                            GetOwner()->GetActorTransform());

    if (spawnedWeapon)
    {
        spawnedWeapon->InitializeMeleeWeapon(weaponCDO->GetMesh()->GetStaticMesh());
        spawnedWeapon->SetActorEnableCollision(false);
        spawnedWeapon->AttachToComponent(GetSKOwnerCharacter()->GetMesh(),
                                         FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");
        spawnedWeapon->FinishSpawning(GetOwner()->GetActorTransform());

        EquippedWeapon = spawnedWeapon;
    }
}

void USKWeaponComponent::DestroyWeapon()
{
    if (!EquippedWeapon) return;

    EquippedWeapon->Destroy();
}

void USKWeaponComponent::SetIsTracingMelee(bool Value)
{
    if (!EquippedWeapon) return;

    switch (Value)
    {
    case true: bIsTracingSword = true; break;

    default:
        bIsTracingSword = false;
        Cast<ASKEquippableSword>(EquippedWeapon)->ResetTraceStartEnd();
        break;
    }
}

void USKWeaponComponent::SetMainWeaponMeshPhysics(const bool IsActive)
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
    }
}

void USKWeaponComponent::SoftResetWeaponTransform(const float Speed)
{
    if (EquippedWeapon && EquippedWeapon->GetStaticMeshComponent()->IsSimulatingPhysics()) return;

    if (Speed <= 0.f)
    {
        EquippedWeapon->SetActorRelativeLocation(FVector::ZeroVector);
        EquippedWeapon->SetActorRelativeRotation(FRotator::ZeroRotator);

        return;
    }

    auto currentLocation = EquippedWeapon->GetRootComponent()->GetRelativeLocation();
    auto currentRotation = EquippedWeapon->GetRootComponent()->GetRelativeRotation();
    if (currentLocation.IsNearlyZero() && currentRotation.IsNearlyZero()) return;

    EquippedWeapon->SetActorRelativeLocation(
        FMath::VInterpConstantTo(currentLocation, FVector::ZeroVector, GetWorld()->GetDeltaSeconds(), Speed));
    EquippedWeapon->SetActorRelativeRotation(
        FMath::RInterpConstantTo(currentRotation, FRotator::ZeroRotator, GetWorld()->GetDeltaSeconds(), Speed));

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, [this, Speed]() { SoftResetWeaponTransform(Speed); }, 1.f, false, .01f);
}