// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Equipables/SKEquippableBase.h"

ASKEquippableBase::ASKEquippableBase()
{
    PrimaryActorTick.bCanEverTick = false;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon mesh");
    BaseMesh->SetupAttachment(RootComponent);
    SetRootComponent(BaseMesh);
}