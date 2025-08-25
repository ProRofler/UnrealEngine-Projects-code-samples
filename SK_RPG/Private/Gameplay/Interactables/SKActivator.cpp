// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKActivator.h"
#include "Gameplay/Interactables/SKDoorway.h"

void ASKActivator::ActivateAll()
{
    for (const auto Actor : ActorsToActivate)
    {
        if (!Actor.IsValid()) continue;

        if (bRandomDelayActivation)
        {
            const auto rand = FMath::RandRange(RandomMin, RandomMin + RandomMaxVariation);

            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, Actor]() {
                                                       Actor->OnInteraction_Implementation(this);
                                                   }),
                                                   rand, false);
        }
        else
        {
            Actor->OnInteraction_Implementation(this);
        }
    }
}

bool ASKActivator::IsAnyDoorActive() const
{

    for (const auto Actor : ActorsToActivate)
    {
        if (!Actor.IsValid()) continue;
    
        if (Actor->GetIsActive()) return true;
    }

    return false;
}
