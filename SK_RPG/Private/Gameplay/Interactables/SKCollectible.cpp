// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKCollectible.h"
#include "Async/Async.h"
#include "Logging/StructuredLog.h"

#include "Kismet/GameplayStatics.h"


ASKCollectible::ASKCollectible()
{
    PrimaryActorTick.bCanEverTick = true;

    BaseMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
    BaseMesh->SetSimulatePhysics(true);
    SetActorEnableCollision(true);

    // BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
    BaseMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    BaseMesh->BodyInstance.bStartAwake = true;

    BaseMesh->SetLinearDamping(2.f);
    BaseMesh->SetAngularDamping(2.f);
}

void ASKCollectible::OnInteraction_Implementation(const AActor *TriggeredActor)
{
    if (!TriggeredActor) return;
    Destroy();
}
