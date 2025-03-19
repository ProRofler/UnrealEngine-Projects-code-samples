// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Equipables/SKEquippableBase.h"

ASKEquippableBase::ASKEquippableBase()
{
    PrimaryActorTick.bCanEverTick = false;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon mesh");
    SetRootComponent(BaseMesh);
}

UStaticMesh *ASKEquippableBase::GetStaticMesh() const { return BaseMesh ? BaseMesh->GetStaticMesh() : nullptr; }

void ASKEquippableBase::BeginPlay() { Super::BeginPlay(); }
