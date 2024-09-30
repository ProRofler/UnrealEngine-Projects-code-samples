// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Props/SKCollectible.h"
#include "Characters/SKBaseCharacter.h"
#include "Logging/StructuredLog.h"

ASKCollectible::ASKCollectible()
{
    //
    BaseMesh->SetSimulatePhysics(true);
}

void ASKCollectible::OnInteraction_Implementation(const AActor *TriggeredActor)
{
    // UE_LOGFMT(LogTemp, Display, "{0} Tried to take {1}", TriggeredActor->GetName(), this->GetName());
    Destroy();
}
