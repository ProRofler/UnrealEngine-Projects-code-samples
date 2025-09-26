// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/S_PlayerWeaponComponent.h"
#include "Animation/S_WeaponSwitchNotify.h"
#include "GameFramework/Character.h"
#include "Weapons/S_BaseWeapon.h"

// Constructor
US_PlayerWeaponComponent::US_PlayerWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void US_PlayerWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapons();
    InitAnimations();
}

void US_PlayerWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // PrimaryWeapon->StopFire();
    // FTransform WeaponTransform = PrimaryWeapon->WeaponMeshComponent->GetComponentTransform();
    SecondaryWeapon->Destroy();
    // PrimaryWeapon = nullptr;
    // SecondaryWeapon = nullptr; //do not assign nullptr after destroying an actor, it'll cause an error
}

void US_PlayerWeaponComponent::AttachWeapons()
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    PrimaryWeapon->SetOwner(Character);
    SecondaryWeapon->SetOwner(Character);

    PrimaryWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
                                     PrimaryWeaponSocketName);

    SecondaryWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
                                       SecondarySocketName);
}

bool US_PlayerWeaponComponent::GetWeaponUIData(FUIData &UIData)
{
    if (PrimaryWeapon)
    {
        UIData = PrimaryWeapon->GetWeaponUIData();
        return true;
    }
    return false;
}

bool US_PlayerWeaponComponent::GetWeaponAmmoData(FAmmoData &AmmoData)
{
    if (PrimaryWeapon)
    {
        AmmoData = PrimaryWeapon->AmmoLog();
        return true;
    }
    return false;
}

FWeaponData US_PlayerWeaponComponent::GetWeaponData()
{
    return WeaponData;
}

void US_PlayerWeaponComponent::SpawnWeapons()
{

    if (!GetWorld())
        return;

    // setting up primary weapon
    PrimaryWeapon = GetWorld()->SpawnActor<AS_BaseWeapon>(WeaponData.PrimaryWeaponClass);
    SecondaryWeapon = GetWorld()->SpawnActor<AS_BaseWeapon>(WeaponData.SecondaryWeaponClass);
    // set up reload animations
    PrimaryReloadAnimation = WeaponData.PrimaryRealoadAnimation;
    SecondaryReloadAnimation = WeaponData.SecondaryRealoadAnimation;
    CurrentReloadAnimation = PrimaryReloadAnimation;

    if (!PrimaryWeapon || !SecondaryWeapon) // check if any weapons selected by user in blueprint
    {
        UE_LOG(LogTemp, Error, TEXT("NOT ALL WEAPON SLOTS FILLED"));
        return;
    }

    AttachWeapons();
}

void US_PlayerWeaponComponent::Fire()
{
    if (!CanShoot())
        return;
    PrimaryWeapon->Fire();
}

void US_PlayerWeaponComponent::StopFire()
{
    if (!PrimaryWeapon)
        return;
    PrimaryWeapon->StopFire();
}

void US_PlayerWeaponComponent::SwitchWeapon()
{
    if (!PrimaryWeapon || !SecondaryWeapon || !CanSwitch())
        return;
    // UE_LOG(LogTemp, Display, TEXT("Weapon switch triggered"));

    bCanShoot = false;
    PrimaryWeapon->StopFire();
    PlayAnimationMontage(WeaponSwitchAnimation);
    Swap(PrimaryWeapon, SecondaryWeapon);
    Swap(PrimaryReloadAnimation, SecondaryReloadAnimation);
    CurrentReloadAnimation = PrimaryReloadAnimation;
    AttachWeapons();
}

void US_PlayerWeaponComponent::PlayAnimationMontage(UAnimMontage *Animation)
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    // UE_LOG(LogTemp, Display, TEXT("Weapon switch montage triggered"));
    Character->PlayAnimMontage(Animation);
}

void US_PlayerWeaponComponent::InitAnimations()
{
    if (!WeaponSwitchAnimation)
        return;

    const auto NotifiesList = WeaponSwitchAnimation->Notifies;

    for (auto NotifyEvent : NotifiesList)
    {
        auto WeaponSwitchNotify = Cast<US_WeaponSwitchNotify>(NotifyEvent.Notify);
        if (WeaponSwitchNotify)
        {
            WeaponSwitchNotify->OnNotified.AddUObject(this, &US_PlayerWeaponComponent::OnEquipFinished);
            break;
        }
    }
}

void US_PlayerWeaponComponent::OnEquipFinished(USkeletalMeshComponent *MeshComp)
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComp)
        return;

    bCanShoot = true;

    // UE_LOG(LogTemp, Display, TEXT("Equip finished"));
}

void US_PlayerWeaponComponent::Reload()
{
    if (!PrimaryWeapon || !SecondaryWeapon || !CanSwitch())
        return;

    if (PrimaryWeapon->IsClipFull() || PrimaryWeapon->IsAmmoEmpty())
    {
        //UE_LOG(LogTemp, Display, TEXT("Clip is full or ammo is empty"));
        return;
    }
    PlayAnimationMontage(CurrentReloadAnimation);
    PrimaryWeapon->Reload();
    //UE_LOG(LogTemp, Display, TEXT("Reload triggered"));
}

void US_PlayerWeaponComponent::DetachMainWeapon()
{
    PrimaryWeapon->WeaponMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    PrimaryWeapon->WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    PrimaryWeapon->WeaponMeshComponent->SetSimulatePhysics(true);
    PrimaryWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    PrimaryWeapon->SetLifeSpan(10.0f);
}

bool US_PlayerWeaponComponent::IsPrimaryAmmoEmpty()
{
    return (PrimaryWeapon->IsClipEmpty() && PrimaryWeapon->IsAmmoEmpty());
}

bool US_PlayerWeaponComponent::IsSecondaryAmmoEmpty()
{
    return (SecondaryWeapon->IsClipEmpty() && SecondaryWeapon->IsAmmoEmpty());
}

bool US_PlayerWeaponComponent::IsAmmoEmpty()
{
    return PrimaryWeapon->IsClipEmpty() && PrimaryWeapon->IsAmmoEmpty() && SecondaryWeapon->IsClipEmpty() &&
           SecondaryWeapon->IsAmmoEmpty();
}
