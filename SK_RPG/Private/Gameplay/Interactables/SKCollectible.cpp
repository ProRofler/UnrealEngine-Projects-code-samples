// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKCollectible.h"
#include "Logging/StructuredLog.h"

ASKCollectible::ASKCollectible()
{
    BaseMesh->SetSimulatePhysics(true);
    SetActorEnableCollision(true);

    BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void ASKCollectible::OnInteraction_Implementation(const AActor *TriggeredActor)
{
    if (!TriggeredActor) return;
    Destroy();
}
