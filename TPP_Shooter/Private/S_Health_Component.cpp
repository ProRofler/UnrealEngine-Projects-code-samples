// Fill out your copyright notice in the Description page of Project Settings.

#include "S_Health_Component.h"
#include "Dev/S_TestDamageType.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraShakeBase.h"

DEFINE_LOG_CATEGORY(LogCharacterDamage);

// Sets default values for this component's properties
US_Health_Component::US_Health_Component()
{
    PrimaryComponentTick.bCanEverTick = false;

}

bool US_Health_Component::IsDead() const
{
    if (FMath::IsNearlyZero(health))
    {
        OnDeath.Broadcast();
        GetOwner()->OnTakeAnyDamage.RemoveAll(this);
        return true;
    }
    else
    {
        return false;
    }
}

// Called when the game starts
void US_Health_Component::BeginPlay()
{
    Super::BeginPlay();

    health = startHealth;
    check(startHealth > 0);

    if(GodMode) health = FMath::Clamp(999.0f, 999.0f, 999.0f);

    AActor *ComponentOwner = GetOwner();
    OnHealthChange.Broadcast(health, 1.0f);

    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &US_Health_Component::HandleDamage);
    }
}

void US_Health_Component::HandleDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                       AController *InstigatedBy, AActor *DamageCauser)
{

    if (IsDead() || GodMode)
    {
        //UE_LOG(LogCharacterDamage, Display, TEXT("Character %s has died"), *DamagedActor->GetName());
        return;
    }

    //else if (DamageType->IsA<US_TestDamageType>())
    //{
    //    //UE_LOG(LogCharacterDamage, Display, TEXT("Taken %.2f of TEST damage by %s"), Damage, *DamageCauser->GetName());
    //}
    //else
    //{
    //    if (DamageCauser)
    //    {
    //        //UE_LOG(LogCharacterDamage, Display, TEXT("Taken %.2f of UNKNOWN damage by %s"), Damage,
    //               *DamageCauser->GetName();
    //    }

    //    else
    //    {
    //        //UE_LOG(LogCharacterDamage, Display, TEXT("Taken %.2f of UNKNOWN damage by environment"), Damage);
    //    }
    //}

    GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
    const float OldHealth = health;
    const float NewHealth = health - Damage;
    SetHealth(NewHealth);
    const float HealthDelta = NewHealth - OldHealth;
    OnHealthChange.Broadcast(health, HealthDelta);
    ShakeCamera();

    if (IsDead())
    {
        //UE_LOG(LogCharacterDamage, Display, TEXT("Character %s has died"), *DamagedActor->GetName());
        return;
    }
    else if (AutoHeal && GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &US_Health_Component::StartAutoHeal, HealRate,
                                               true, HealDelay);
    }
}

void US_Health_Component::StartAutoHeal()
{
    if (health >= startHealth || IsDead() || !GetWorld())
        return;

    SetHealth(health + HealAmount);
    OnHealthChange.Broadcast(health, 100.0f);
    // UE_LOG(LogCharacterDamage, Display, TEXT("Auto restored %.2f health"), HealAmount);
}

void US_Health_Component::SetHealth(float HealthAmount)
{
    health = FMath::Clamp(HealthAmount, 0.0f, startHealth);
    if (FMath::IsNearlyEqual(health, startHealth))
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
        return;
    }
}

void US_Health_Component::ShakeCamera()
{
    if(IsDead()) return;
    const auto Pawn = Cast<APawn>(GetOwner());
    if(!Pawn) return;
    const auto Controller = Pawn->GetController<APlayerController>();
    if(!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void US_Health_Component::AddHealth(float HealthAmount)
{
    health = FMath::Clamp(health + HealthAmount, 0.0f, startHealth);
}
