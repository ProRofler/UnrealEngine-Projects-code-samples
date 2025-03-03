// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKCharacterComponentBase.h"

#include "Characters/SKBaseCharacter.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

USKCharacterComponentBase::USKCharacterComponentBase() { PrimaryComponentTick.bCanEverTick = false; }

// Called when the game starts
void USKCharacterComponentBase::BeginPlay()
{
    Super::BeginPlay();
    InitializeSKCharacterOwner();
}

void USKCharacterComponentBase::InitializeSKCharacterOwner()
{
    if (GetOwner()->IsA<ASKBaseCharacter>())
    {
        SKCharacter = Cast<ASKBaseCharacter>(GetOwner());

        UE_LOGFMT(LogSKCharacterComponent, Display, "{1} owner init success, new owner is {2}", ("1", GetName()),
                  ("2", SKCharacter->GetName()));

        return;
    }

    UE_LOGFMT(LogSKCharacterComponent, Error, "Couldn't initialize owner for the character component!");
    checkNoEntry();
}
