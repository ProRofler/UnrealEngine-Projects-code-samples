// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/Components/SKLockComponent.h"
#include "Gameplay/Interactables/SKKeyItem.h"

#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/SKBaseCharacter.h"
#include "Characters/SKPlayerCharacter.h"

#include "Core/Interface/SKInterfaceCharacter.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

USKLockComponent::USKLockComponent() { PrimaryComponentTick.bCanEverTick = false; }

const bool USKLockComponent::TryUnlocking(const AActor *UnlockInitiator)
{
    const bool hasKey =
        ISKInterfaceCharacter::Execute_GetInventoryComponent(UnlockInitiator)->IsInInventoryByClass(KeyClass);

    if (hasKey)
    {
        Unlock(UnlockInitiator);
        return true;
    }

    return false;
}

void USKLockComponent::BeginPlay()
{
    Super::BeginPlay();

    if (bIsLocked && !KeyClass)
    {
        UE_LOGFMT(LogSKInteractions, Warning, "{1} lock is active but no key class selected, is this intentional?",
                  ("1", GetOwner()->GetName()));
    }
}
