// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKKeyItem.h"
#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

ASKKeyItem::ASKKeyItem() { SetCollectibleType(ECollectibleType::Key); }

void ASKKeyItem::BeginPlay() { Super::BeginPlay(); }
