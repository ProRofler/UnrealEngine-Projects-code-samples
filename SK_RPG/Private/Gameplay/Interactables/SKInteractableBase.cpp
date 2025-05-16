// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKInteractableBase.h"
#include "Characters/SKBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

ASKInteractableBase::ASKInteractableBase()
{
    PrimaryActorTick.bCanEverTick = false;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Object mesh");
    SetRootComponent(BaseMesh);

    BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BaseMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    InteractableID = FGuid::NewGuid();
}

void ASKInteractableBase::BeginPlay() { Super::BeginPlay(); }

#if !UE_BUILD_SHIPPING
void ASKInteractableBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

#if WITH_EDITORONLY_DATA
    if (bDrawCenterSphere) DrawCenter();
#endif
}
#endif

void ASKInteractableBase::DrawCenter()
{
    FVector center, extend;
    GetActorBounds(false, center, extend, true);

    DrawDebugSphere(GetWorld(), center, 8.0f, 8, FColor::Green);
}

void ASKInteractableBase::OnInteraction_Implementation(const AActor *TriggeredActor)
{
    UE_LOGFMT(
        LogSKInteractions, Warning,
        "Attempt to call an empty method of a base class in {1}, is this intended? This method should not be called",
        ("1", GetName()));
}
