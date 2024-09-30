// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/S_PlayerWidget.h"
#include "S_Health_Component.h"
// #include "GameFramework/SpectatorPawn.h"
#include "GameFramework/PlayerState.h"
#include "S_PlayerWeaponComponent.h"
#include "TUtils.h"

bool US_PlayerWidget::Initialize()
{
    const auto PlayerPawn = GetOwningPlayerPawn();
    if (!PlayerPawn)
        return Super::Initialize();
    const auto Component = PlayerPawn->GetComponentByClass(US_Health_Component::StaticClass());
    if (!PlayerPawn)
        return Super::Initialize();
    const auto HealthComponent = Cast<US_Health_Component>(Component);
    if (HealthComponent)
    {
        HealthComponent->OnHealthChange.AddUObject(this, &US_PlayerWidget::OnTakeDamage);
    }

    return Super::Initialize();
}

void US_PlayerWidget::OnTakeDamage(float Health, float HealthDelta)
{
    if (HealthDelta < 0)
    {
        UE_LOG(LogTemp, Display, TEXT("Event on damage called"));
        OnHealthChanged();
    }
}

float US_PlayerWidget::GetHealthPercent() const
{

    const auto PlayerPawn = GetOwningPlayerPawn();
    if (!PlayerPawn)
        return 5.5f;

    const auto Component = PlayerPawn->GetComponentByClass(US_Health_Component::StaticClass());
    const auto HealthComponent = Cast<US_Health_Component>(Component);
    if (!HealthComponent)
        return 15.5f;

    return HealthComponent->GetHealthPercent();
}

bool US_PlayerWidget::GetWeaponUIData(FUIData &UIData) const
{
    const auto Weapon = TSUtils::GetComponent<US_PlayerWeaponComponent>(GetOwningPlayerPawn());
    if (!Weapon)
        return false;

    return Weapon->GetWeaponUIData(UIData);
}

bool US_PlayerWidget::GetWeaponAmmoData(FAmmoData &AmmoData) const
{
    const auto Weapon = TSUtils::GetComponent<US_PlayerWeaponComponent>(GetOwningPlayerPawn());
    if (!Weapon)
        return false;

    return Weapon->GetWeaponAmmoData(AmmoData);
}

bool US_PlayerWidget::isPlayerSpectating() const
{
    const auto Player = GetOwningPlayer();
    return Player && Player->GetStateName() == NAME_Spectating;
}
