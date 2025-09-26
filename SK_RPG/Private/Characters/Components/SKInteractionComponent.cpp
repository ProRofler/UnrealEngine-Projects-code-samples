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

#include "Kismet/KismetMathLibrary.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"

#include "Gameplay/GAS/SKNativeGameplayTags.h"
#include "Gameplay/GAS/SKAbilitySystemComponent.h"

#include "Utils/Libraries/SKFunctionLibrary.h"

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

    OnVicinityChanged.BindUObject(this, &USKInteractionComponent::HandleVicinityChanged);
}

void USKInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    check(OnVicinityChanged.IsBound());

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
            }
        }
    }

    if (!InteractablesInVicinity.IsEmpty())
    {
        OnVicinityChanged.Execute();
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
            OnVicinityChanged.Execute();
            ISKInterfaceInteractable::Execute_SetIsInReach(OtherActor, true);
        }
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
            OnVicinityChanged.Execute();
            ISKInterfaceInteractable::Execute_SetIsInReach(OtherActor, false);
        }
        InteractionTarget = nullptr;
    }
}

void USKInteractionComponent::HandleVicinityChanged()
{
    if (!bIsActiveAtomic.load())
    {
        bIsActiveAtomic.store(true);
        AsyncInteractionHandle();
    }

    else if (InteractablesInVicinity.IsEmpty())
    {
        bIsActiveAtomic.store(false);
    }
}

// big ass method. refactor?
void USKInteractionComponent::AsyncInteractionHandle()
{
#if !UE_BUILD_SHIPPING
    TRACE_CPUPROFILER_EVENT_SCOPE_STR("USKInteractionComponent::AsyncInteractionHandle");
#endif

    Async(EAsyncExecution::Thread, [this, WeakOwner = TWeakObjectPtr<AActor>(GetOwner())]() {
        while (bIsActiveAtomic.load())
        {
            if (!WeakOwner.IsValid()) break;

            const auto OwnerCharacter = Cast<ASKPlayerCharacter>(WeakOwner.Get());
            const auto ASC =
                Cast<USKAbilitySystemComponent>(OwnerCharacter ? OwnerCharacter->GetAbilitySystemComponent() : nullptr);

            if (!ASC || !OwnerCharacter)
            {
                UE_LOGFMT(LogSKInteractions, Error, "Owner or ASC is invalid in AsyncInteractionHandle");
                break;
            }

            if (ASC->HasMatchingGameplayTag(FSKGameplayTags::Get().Character_State_Action_GrabbingItem))
            {
                AsyncTask(ENamedThreads::GameThread, [this, ASC]() {
                    ASC->CheckAndRemoveGameplayTag(FSKGameplayTags::Get().Character_State_Action_CanInteract);
                    InteractionTarget = nullptr;
                });

                FPlatformProcess::Sleep(0.02f); // Sleep to reduce CPU usage
                continue;
            }

            TArray<TWeakObjectPtr<AActor>> InteractablesSnapshot;

            if (DataGuard.TryReadLock())
            {
                for (auto &Actor : InteractablesInVicinity)
                {
                    InteractablesSnapshot.Add(Actor);
                }
                DataGuard.ReadUnlock();
            }

            // Game Thread
            AsyncTask(ENamedThreads::GameThread, [this, ASC, InteractablesSnapshot, OwnerCharacter]() {
                const auto tagCanInteract = FSKGameplayTags::Get().Character_State_Action_CanInteract;

                AActor *BestTarget = nullptr;
                double BestDot = -1.0;
                double Threshold = 0.0;

                const FVector ViewLocation = OwnerCharacter->PlayerCamera->GetComponentLocation();
                const FVector ViewDirection = OwnerCharacter->PlayerCamera->GetForwardVector();

                for (const auto &WeakActor : InteractablesSnapshot)
                {
                    if (!WeakActor.IsValid()) continue;

                    FVector Origin, Extent;
                    WeakActor->GetActorBounds(false, Origin, Extent);
                    ISKInterfaceInteractable::Execute_SetIsFocused(WeakActor.Get(), false);

                    const FVector ToTarget = UKismetMathLibrary::GetDirectionUnitVector(ViewLocation, Origin);
                    const double Dot = FVector::DotProduct(ViewDirection, ToTarget);

                    if (Dot > BestDot)
                    {
                        BestDot = Dot;
                        BestTarget = WeakActor.Get();

                        const double DistanceSqr = FVector::DistSquared(ViewLocation, Origin);
                        const double ClampedDist = FMath::Clamp(DistanceSqr * 0.000002f + 0.95f, 0.0f, 0.99f);
                        const double BoundsDelta = (Extent.GetAbsMax() * 0.002f);
                        Threshold = ClampedDist - BoundsDelta;
                    }
                }

                if (BestDot < Threshold || !BestTarget)
                {
                    InteractionTarget = nullptr;
                    ASC->CheckAndRemoveGameplayTag(tagCanInteract);
                    return;
                }

                const FVector traceStart = Cast<ASKPlayerCharacter>(GetOwner())->PlayerCamera->GetComponentLocation();
                FHitResult HitCheck = USKFunctionLibrary::TraceToBoundingBox(GetWorld(), traceStart, BestTarget);

                if (!HitCheck.bBlockingHit)
                {
                    if (!OwnerCharacter->TraceFromCamera(HitCheck, GrabDistance))
                    {
                        InteractionTarget = nullptr;
                        ASC->CheckAndRemoveGameplayTag(tagCanInteract);
                        return;
                    }
                }

                AActor *Traced = HitCheck.GetActor();
                if (Traced && Traced->Implements<USKInterfaceInteractable>())
                {
                    InteractionTarget = Traced;
                    ISKInterfaceInteractable::Execute_SetIsFocused(Traced, true);

                    ASC->CheckAndAddGameplayTag(tagCanInteract);
                }
                else
                {
                    InteractionTarget = nullptr;
                    ASC->CheckAndRemoveGameplayTag(tagCanInteract);
                }
            });

            FPlatformProcess::Sleep(0.02f); // 50 FPS polling
        }

        // To clear interaction target if the loop is done
        AsyncTask(ENamedThreads::GameThread, [this]() { InteractionTarget = nullptr; });
    });
}

void USKInteractionComponent::Interact()
{
    if (!InteractionTarget.IsValid() || !InteractionTarget->Implements<USKInterfaceInteractable>()) return;

    const bool bLogEnabled =
        GetSKOwnerCharacter()->bEnableLogging && GetSKOwnerCharacter()->bEnableLoggingAbilitySystem;
    if (bLogEnabled)
        UE_LOGFMT(LogSKAbilitySystem, Display, "'{ActorName}' tried to interact with '{InteractableName}'!",
                  ("ActorName", GetOwner()->GetName()), ("InteractableName", InteractionTarget->GetName()));

    if (InteractionTarget->Implements<USKInterfaceCollectible>())
    {
        const auto InventoryComponent = ISKInterfaceCharacter::Execute_GetInventoryComponent(GetOwner());
        InventoryComponent->AddToInventory(InteractionTarget.Get());
    }

    ISKInterfaceInteractable::Execute_OnInteraction(InteractionTarget.Get(), GetOwner());
}
