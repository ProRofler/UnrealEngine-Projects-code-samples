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
    Destroy();
}
