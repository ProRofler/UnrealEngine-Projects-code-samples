// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Props/SKCollectible.h"
#include "Logging/StructuredLog.h"

ASKCollectible::ASKCollectible()
{
    BaseMesh->SetSimulatePhysics(true);
    SetActorEnableCollision(true);
}

void ASKCollectible::OnInteraction_Implementation(const AActor *TriggeredActor)
{
    if (!TriggeredActor) return;

    switch (bIsInInventory)
    {
    case 0:
        HideFromWorld();
        bIsInInventory = true;
        break;
    default:
        ShowInWorld();
        bIsInInventory = false;
        break;
    }
}

void ASKCollectible::HideFromWorld()
{
    SetActorHiddenInGame(true);
    BaseMesh->SetSimulatePhysics(false);
    SetActorEnableCollision(false);

    // ADD LOG
}

void ASKCollectible::ShowInWorld()
{
    BaseMesh->SetSimulatePhysics(true);
    SetActorEnableCollision(true);
    SetActorHiddenInGame(false);

    // ADD LOG
}
