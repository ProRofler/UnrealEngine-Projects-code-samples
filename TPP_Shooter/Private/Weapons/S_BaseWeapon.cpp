
#include "Weapons/S_BaseWeapon.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "Weapons/S_FXWeaponComponent.h"

// Sets default values
AS_BaseWeapon::AS_BaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;
    WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon mesh");
    // WeaponMeshComponent->SetSimulatePhysics(true);
    SetRootComponent(WeaponMeshComponent);
    FXComponent = CreateDefaultSubobject<US_FXWeaponComponent>("Weapon FX");
}

// Called when the game starts or when spawned
void AS_BaseWeapon::BeginPlay()
{
    CurrentAmmo = AmmoSettings;
    // AmmoLog();
    Super::BeginPlay();
}

void AS_BaseWeapon::Fire()
{
}

void AS_BaseWeapon::StopFire()
{
}

void AS_BaseWeapon::AddAmmo(int32 AmmoAmount)
{
    UE_LOG(LogTemp, Display, TEXT("Add ammo called"));
    CurrentAmmo.AmmoTotal += AmmoAmount;
    CurrentAmmo.AmmoTotal = FMath::Clamp(CurrentAmmo.AmmoTotal, 0, AmmoSettings.AmmoMax);
}

void AS_BaseWeapon::Shot()
{
}

void AS_BaseWeapon::DecreaseAmmo()
{
    --CurrentAmmo.AmmoCurrent;
    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        // Reload();
        AmmoLog();
        return;
    }
    AmmoLog();
}

FHitResult AS_BaseWeapon::TraceFromCamera()
{
    if (!GetWorld())
        return FHitResult();

    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return FHitResult();

    const auto Controller = Player->GetController();
    if (!Controller)
        return FHitResult();

    FVector PlayerLocation;
    FRotator PlayerRotation;

    const auto CharCheck = Cast<ACharacter>(GetOwner());
    if (!CharCheck)
        return FHitResult();

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
    FVector TraceDirection =
        PlayerRotation.Vector() + RandomDirection; // SocketTransform.GetRotation().GetForwardVector();
    FVector TraceEnd = PlayerLocation + TraceDirection * 5000.0f;

    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, PlayerLocation, TraceEnd, ECollisionChannel::ECC_Visibility,
                                         TraceParams);

    return HitResult;
}

void AS_BaseWeapon::Reload()
{
    if (CurrentAmmo.AmmoTotal <= 0)
    {
        //UE_LOG(LogTemp, Display, TEXT("---NO AMMO---"));
        return;
    }
    if (IsClipFull() || IsAmmoEmpty())
        return;

    //UE_LOG(LogTemp, Display, TEXT("---RELOAD---"));
    check(CurrentAmmo.AmmoCurrent + CurrentAmmo.AmmoTotal != 0);

    auto ReloadNeeded = CurrentAmmo.ClipSize - CurrentAmmo.AmmoCurrent;
    auto ReloadAmount = CurrentAmmo.AmmoTotal >= ReloadNeeded ? ReloadNeeded : CurrentAmmo.AmmoTotal;

    CurrentAmmo.AmmoCurrent += ReloadAmount;
    CurrentAmmo.AmmoTotal =
        CurrentAmmo.bInfiniteAmmo == false ? CurrentAmmo.AmmoTotal - ReloadAmount : CurrentAmmo.AmmoTotal = 999;
    AmmoLog();
}

UNiagaraComponent *AS_BaseWeapon::SpawnNiagaraFX()
{
    if (FXComponent->MuzzleFlashFX)
        return UNiagaraFunctionLibrary::SpawnSystemAttached(FXComponent->MuzzleFlashFX, WeaponMeshComponent,
                                                            MuzzleSocket, FVector::ZeroVector, FRotator::ZeroRotator,
                                                            EAttachLocation::SnapToTarget, true);

    else
        return nullptr;
}