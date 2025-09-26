// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/S_PickupHealth.h"
#include "S_Health_Component.h"

void AMyS_PickupHealth::OnPickup(AActor *PlayerActor)
{
    //UE_LOG(LogTemp, Display, TEXT("Picked up - %s by %s"), *GetActorNameOrLabel(), *PlayerActor->GetActorNameOrLabel());

    const auto HealthComponent = PlayerActor->GetComponentByClass<US_Health_Component>();
    if(!HealthComponent || HealthComponent->IsDead()) return;

    HealthComponent->AddHealth(HealthAmount);
}
