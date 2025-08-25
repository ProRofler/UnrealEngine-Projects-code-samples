// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Core/Performace/SKSignificanceManagerComponent.h"

#include "Core/SKLogCategories.h"
#include "Gameplay/Interactables/SKInteractableBase.h"
#include "Logging/StructuredLog.h"

USKSignificanceManagerComponent::USKSignificanceManagerComponent() { PrimaryComponentTick.bCanEverTick = true; }

void USKSignificanceManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!Cast<ASKInteractableBase>(GetOwner()))
    {
        UE_LOGFMT(LogSKSignificance, Warning, "Class: {1} is incompatible with SKSignificanceManager component",
                  ("1", GetClass()->GetName()));
    }

    USignificanceManager *SignificanceManager = USignificanceManager::Get(GetWorld());
    if (!SignificanceManager) return;

    auto Significance = [&](USignificanceManager::FManagedObjectInfo *ObjectInfo,
                            const FTransform &Viewpoint) -> float {
        return SignficanceFunction(ObjectInfo, Viewpoint);
    };

    auto PostSignificance = [&](USignificanceManager::FManagedObjectInfo *ObjectInfo, float OldSignificance,
                                float Significance, bool bFinal) {
        PostSignficanceFunction(ObjectInfo, OldSignificance, Significance, bFinal);
    };

    SignificanceManager->RegisterObject(GetOwner(), *GetOwner()->GetName(), Significance,
                                        USignificanceManager::EPostSignificanceType::Sequential, PostSignificance);
}

void USKSignificanceManagerComponent::EndPlay(const EEndPlayReason::Type Reason)
{
    Super::EndPlay(Reason);

    if (GetWorld())
    {
        if (auto SignificanceManager = USignificanceManager::Get(GetWorld()))
            SignificanceManager->UnregisterObject(GetOwner());
    }
}

float USKSignificanceManagerComponent::SignficanceFunction(USignificanceManager::FManagedObjectInfo *ObjectInfo,
                                                           const FTransform &Viewpoint)
{
    if (ObjectInfo->GetTag() == *GetOwner()->GetName())
    {
        ASKInteractableBase *Actor = Cast<ASKInteractableBase>(ObjectInfo->GetObject());
        if (!Actor) return 0.f;

        const float Distance = (Actor->GetActorLocation() - Viewpoint.GetLocation()).Size();

        const float amountDelta = FMath::GetMappedRangeValueClamped(TRange<float>(0.f, 300.f), TRange<float>(1.f, 4.f),
                                                                    (float)Actor->GetInteractablesCount());

        const float adjustedMaxDistance = 2500.f / amountDelta;

        if (Distance >= adjustedMaxDistance && Actor->GetMesh()->GetComponentVelocity().Size() < 10.f) return 2.f;
        if (Distance >= adjustedMaxDistance) return 1.f;
    }

    return 0.f;
}

void USKSignificanceManagerComponent::PostSignficanceFunction(USignificanceManager::FManagedObjectInfo *ObjectInfo,
                                                              float OldSignificance, float Significance, bool bFinal)
{
    if (ObjectInfo->GetTag() == *GetOwner()->GetName())
    {
        if (Significance >= 2.f)
        {
            SetSignificanceMode(ESignificancePreset::Low);
        }
        else if (Significance >= 1.f)
        {
            SetSignificanceMode(ESignificancePreset::Medium);
        }
        else
        {
            SetSignificanceMode(ESignificancePreset::High);
        }
    }

// DEBUG
#if 0
    ASKInteractableBase *Actor = Cast<ASKInteractableBase>(ObjectInfo->GetObject());
    if (!Actor) return;
    GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue,
                                     GetName() + " Significance: " + FString::SanitizeFloat(Significance) +
                                         " total amount: " + FString::FromInt(Actor->GetInteractablesCount()),
                                     true);
#endif
}

void USKSignificanceManagerComponent::SetSignificanceMode(const ESignificancePreset Preset)
{
    ASKInteractableBase *Actor = Cast<ASKInteractableBase>(GetOwner());
    UStaticMeshComponent *mesh = Actor->GetMesh();
    if (!Actor || !mesh) return;

    switch (Preset)
    {
    case ESignificancePreset::High:
        Actor->SetActorTickInterval(0.f);

        if (mesh->IsSimulatingPhysics() && !mesh->RigidBodyIsAwake())
        {
            mesh->BodyInstance.WakeInstance();

            mesh->BodyInstance.LinearDamping = Actor->InitialLinearDampening;
            mesh->BodyInstance.AngularDamping = Actor->InitialAngularDampening;
        }

        mesh->SetCastShadow(true);
        mesh->SetGenerateOverlapEvents(true);

        break;
    case ESignificancePreset::Medium:
        Actor->SetActorTickInterval(0.1f);

        mesh->SetCastShadow(false);
        mesh->SetGenerateOverlapEvents(false);

        if (mesh->IsSimulatingPhysics() && !mesh->RigidBodyIsAwake())
        {
            mesh->BodyInstance.LinearDamping = Actor->InitialLinearDampening * 10;
            mesh->BodyInstance.AngularDamping = Actor->InitialAngularDampening * 2;
        }

        break;
    case ESignificancePreset::Low:
        Actor->SetActorTickInterval(0.25f);

        mesh->SetCastShadow(false);
        mesh->SetGenerateOverlapEvents(false);

        if (mesh->IsSimulatingPhysics() && !mesh->RigidBodyIsAwake())
        {
            mesh->BodyInstance.LinearDamping = Actor->InitialLinearDampening * 10;
            mesh->BodyInstance.AngularDamping = Actor->InitialAngularDampening * 2;

            mesh->PutRigidBodyToSleep();
        }

        break;
    default: checkNoEntry(); break;
    }
}
