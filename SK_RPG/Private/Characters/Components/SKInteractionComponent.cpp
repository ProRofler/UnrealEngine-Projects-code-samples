// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKInteractionComponent.h"

#include "Characters/SKBaseCharacter.h"
#include "Characters/SKPlayerCharacter.h"

#include "Characters/Components/SKInventoryComponent.h"

#include "Components/CapsuleComponent.h"
#include "Core/Interface/SKInterfaceCollectible.h"
#include "Core/Interface/SKInterfaceInteractable.h"

#include "Core/Interface/SKInterfaceCharacter.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

#include "Gameplay/GAS/SKNativeGameplayTags.h"

#include "Kismet/KismetMathLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Camera/CameraComponent.h"

#include "Gameplay/GAS/SKAbilitySystemComponent.h"

#include "ProfilingDebugging/CpuProfilerTrace.h"

USKInteractionComponent::USKInteractionComponent()
{
    // Interactions system
    InteractionZone = CreateDefaultSubobject<UCapsuleComponent>("Interactive zone");
    InteractionZone->SetCapsuleHalfHeight(130.0f);
    InteractionZone->SetCapsuleRadius(100.0f);
    InteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    InteractionZone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    InteractionZone->SetGenerateOverlapEvents(true);
    InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &USKInteractionComponent::OnBeginOverlap);
    InteractionZone->OnComponentEndOverlap.AddDynamic(this, &USKInteractionComponent::OnOverlapEnd);
}

void USKInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    // InteractionZone->AttachToComponent(GetOwner()->GetRootComponent(),
    //                                    FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    // Check vicinity on start
    TArray<AActor *> OverlappingActors;
    InteractionZone->GetOverlappingActors(OverlappingActors);
    for (const auto OverlappedActor : OverlappingActors)
    {
        if (OverlappedActor->Implements<USKInterfaceInteractable>())
        {
            if (DataGuard.TryWriteLock())
            {
                InteractablesInVicinity.Add(OverlappedActor);
                DataGuard.WriteUnlock();
                HandleInteractionsTimer();
            }
        }
    }
}

void USKInteractionComponent::OnBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                             UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult &SweepResult)
{
    if (OtherActor->Implements<USKInterfaceInteractable>())
    {
        if (DataGuard.TryWriteLock())
        {
            InteractablesInVicinity.Add(OtherActor);
            DataGuard.WriteUnlock();
        }
        HandleInteractionsTimer();
    }
}

void USKInteractionComponent::OnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                           UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    if (InteractablesInVicinity.Contains(OtherActor))
    {
        if (DataGuard.TryWriteLock())
        {
            InteractablesInVicinity.Remove(OtherActor);
            DataGuard.WriteUnlock();
        }
        InteractionTarget = nullptr;
    }
}

void USKInteractionComponent::AsyncInteractionHandle()
{
    Async(EAsyncExecution::TaskGraph, [&]() { HandleInteractionActor(); });
}

void USKInteractionComponent::Interact()
{
    if (!InteractionTarget.IsValid()) return;

    if (GetSKOwnerCharacter()->bEnableLogging && GetSKOwnerCharacter()->bEnableLoggingAbilitySystem)
        UE_LOGFMT(LogSKAbilitySystem, Display, "'{ActorName}' tried to interact with '{InteractableName}'!",
                  ("ActorName", GetOwner()->GetName()), ("InteractableName", InteractionTarget->GetName()));

    if (InteractionTarget->Implements<USKInterfaceCollectible>())
    {
        const auto InventoryComponent = ISKInterfaceCharacter::Execute_GetInventoryComponent(GetOwner());
        InventoryComponent->AddToInventory(InteractionTarget.Get());
    }

    ISKInterfaceInteractable::Execute_OnInteraction(InteractionTarget.Get(), GetOwner());
}

void USKInteractionComponent::HandleInteractionsTimer()
{
    if (!GetWorld()) return;

    if (InteractablesInVicinity.Num() > 0 &&
        GetOwner()->GetWorldTimerManager().IsTimerActive(InteractableActiveUpdateTimer))
    {
        return;
    }

    else if (InteractablesInVicinity.Num() > 0)
    {
        GetOwner()->GetWorldTimerManager().SetTimer(InteractableActiveUpdateTimer, this,
                                                    &USKInteractionComponent::AsyncInteractionHandle, 0.1, true);
    }

    else
    {
        GetOwner()->GetWorldTimerManager().ClearTimer(InteractableActiveUpdateTimer);
    }
}

void USKInteractionComponent::HandleInteractionActor()
{

    if (const auto Player = Cast<ASKPlayerCharacter>(GetOwner()))
    {
        const auto tagCanInteract = FSKGameplayTags::Get().Character_State_Action_CanInteract;

        // No calculation if currently grabbing item
        const auto ASC = Cast<USKAbilitySystemComponent>(GetSKOwnerCharacter()->GetAbilitySystemComponent());

        if (ASC && ASC->HasMatchingGameplayTag(FSKGameplayTags::Get().Character_State_Action_GrabbingItem))
        {
            ASC->CheckAndRemoveGameplayTag(tagCanInteract);
            InteractionTarget = nullptr; // Explicitly clearing interaction target
            return;
        }

        InteractionTarget = GetLookedAtActor();

        if (!InteractionTarget.IsValid())
        {
            ASC->CheckAndRemoveGameplayTag(tagCanInteract);
            return;
        }

        // final check with trace
        FHitResult TraceCheck = TraceToBoundingBox(InteractionTarget.Get());

        if (!TraceCheck.bBlockingHit)
        {
            if (!Cast<ASKPlayerCharacter>(GetOwner())->TraceFromCamera(TraceCheck, GrabDistance))
            {
                InteractionTarget = nullptr;
                return;
            }
        }

        // final comparison
        if (TraceCheck.GetActor() == InteractionTarget || TraceCheck.GetActor()->Implements<USKInterfaceInteractable>())
        {
            InteractionTarget = TraceCheck.GetActor();
            ASC->CheckAndAddGameplayTag(tagCanInteract);
        }
        else
        {
            InteractionTarget = nullptr;
        }
    }
    else
    {
        // TODO: AI logic?
    }
}

FHitResult USKInteractionComponent::TraceToBoundingBox(const AActor *OtherActor) const
{
    FHitResult HitResult;

    const auto tracePoint = OtherActor->GetComponentsBoundingBox().GetCenter();

    // DrawDebugSphere(GetWorld(), tracePoint, 4.0f, 8, FColor::Red, false, 0.1f);

    GetWorld()->LineTraceSingleByChannel(HitResult,
                                         Cast<ASKPlayerCharacter>(GetOwner())->PlayerCamera->GetComponentLocation(),
                                         tracePoint, ECollisionChannel::ECC_Visibility);

    return HitResult;
}

AActor *USKInteractionComponent::GetLookedAtActor() const
{
#if !UE_BUILD_SHIPPING
    TRACE_CPUPROFILER_EVENT_SCOPE_STR("ASKPlayerCharacter::GetLookedAtActor");
#endif

    double BestDotProduct = -1.0f;
    double Threshold = 0.0f;
    AActor *Item = nullptr;

    FRWScopeLock ReadLock(DataGuard, SLT_ReadOnly);

    for (const auto &ItemInVicinity : InteractablesInVicinity)
    {
        // get actor bounds
        FVector ActorBoundsOrigin, ActorBoxExtent;
        ItemInVicinity->GetActorBounds(false, ActorBoundsOrigin, ActorBoxExtent);

        // calcuate dot product
        const auto PC = Cast<ASKPlayerCharacter>(GetOwner());

        const auto DotProduct = FVector::DotProduct(
            PC->PlayerCamera->GetForwardVector(),
            UKismetMathLibrary::GetDirectionUnitVector(PC->PlayerCamera->GetComponentLocation(), ActorBoundsOrigin));

        if (DotProduct >= BestDotProduct)
            BestDotProduct = DotProduct;
        else
            continue;

        // dot product threshold variables
        // magic numbers here were manually assigned for adjusting to an item mesh
        const auto distanceSqr = FVector::DistSquared(PC->PlayerCamera->GetComponentLocation(), ActorBoundsOrigin);

        const auto clampedDistance = FMath::Clamp(distanceSqr * 0.000002f + 0.95f, 0.0f, 0.99f);
        const auto boundsDelta = (ActorBoxExtent.GetAbsMax() * 0.002f);

        // Minimally required dot product value to be considered as if player looking at item
        Threshold = clampedDistance - boundsDelta;

        Item = ItemInVicinity;
    }

    return BestDotProduct < Threshold ? nullptr : Item;
}
