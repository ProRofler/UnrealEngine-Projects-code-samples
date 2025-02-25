// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKKeyItem.h"
#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

void ASKKeyItem::BeginPlay()
{
    Super::BeginPlay();

    if (KeyID == TEXT("None"))
    {
        UE_LOGFMT(LogSKInteractions, Warning, "{1} key ID is None, is this intentional?", ("1", GetName()));
    }
}
