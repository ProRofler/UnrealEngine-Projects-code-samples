// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKInteractableBase.h"
#include "Characters/SKBaseCharacter.h"
#include "Components/CapsuleComponent.h"

ASKInteractableBase::ASKInteractableBase()
{
    PrimaryActorTick.bCanEverTick = false;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Object mesh");
    SetRootComponent(BaseMesh);

    BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BaseMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void ASKInteractableBase::BeginPlay() { Super::BeginPlay(); }

void ASKInteractableBase::OnInteraction_Implementation(const AActor *TriggeredActor)
{
    // This function must not be called, this class is only a base
    checkNoEntry();
}
