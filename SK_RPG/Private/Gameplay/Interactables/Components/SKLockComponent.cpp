// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/Components/SKLockComponent.h"

#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/SKBaseCharacter.h"
#include "Characters/SKPlayerCharacter.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

USKLockComponent::USKLockComponent() { PrimaryComponentTick.bCanEverTick = false; }

const bool USKLockComponent::TryUnlocking(const AActor *UnlockInitiator)
{
    if (!UnlockInitiator) return false;

    if (GetIsLocked())
    {
        UE_LOGFMT(LogSKInteractions, Display, "{1} tried to unlock {2}", ("1", UnlockInitiator->GetName()),
                  ("2", GetOwner()->GetName()));

        if (const auto inventory = UnlockInitiator->FindComponentByClass<USKInventoryComponent>())
        {
            if (inventory->IsInInventory(KeyID))
            {
                Unlock();
                return true;
            }
        }
    }
    else
    {
        return true;
    }

    return false;
}

void USKLockComponent::BeginPlay()
{
    Super::BeginPlay();

    if (KeyID == TEXT("None"))
    {
        UE_LOGFMT(LogSKInteractions, Warning, "{1} lock key ID is None, is this intentional?",
                  ("1", GetOwner()->GetName()));
    }
}
