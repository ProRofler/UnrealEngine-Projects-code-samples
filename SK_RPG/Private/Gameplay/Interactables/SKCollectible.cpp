// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKCollectible.h"
#include "Async/Async.h"
#include "Logging/StructuredLog.h"

#include "Kismet/GameplayStatics.h"

int32 ASKCollectible::CollectiblesAmount = 0;

ASKCollectible::ASKCollectible()
{
    PrimaryActorTick.bCanEverTick = true;

    BaseMesh->SetSimulatePhysics(true);
    BaseMesh->SetLinearDamping(2.f);
    BaseMesh->SetAngularDamping(2.f);
    SetActorEnableCollision(true);

    BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    BaseMesh->BodyInstance.bStartAwake = true;
}

void ASKCollectible::OnInteraction_Implementation(const AActor *TriggeredActor)
{
    if (!TriggeredActor) return;
    Destroy();
}

void ASKCollectible::BeginPlay()
{
    Super::BeginPlay();

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

    SignificanceManager->RegisterObject(this, *GetName(), Significance,
                                        USignificanceManager::EPostSignificanceType::Sequential, PostSignificance);

    InitialLinearDampening = BaseMesh->BodyInstance.LinearDamping;
    InitialAngularDampening = BaseMesh->BodyInstance.AngularDamping;

    CollectiblesAmount++;
}

void ASKCollectible::EndPlay(const EEndPlayReason::Type Reason)
{
    Super::EndPlay(Reason);

    auto WorldPtr = GetWorld();
    if (!WorldPtr) return;

    CollectiblesAmount--;
    if (auto SignificanceManager = USignificanceManager::Get(WorldPtr)) SignificanceManager->UnregisterObject(this);
}

float ASKCollectible::SignficanceFunction(USignificanceManager::FManagedObjectInfo *ObjectInfo,
                                          const FTransform &Viewpoint)
{
    if (ObjectInfo->GetTag() == *GetName())
    {
        ASKCollectible *Actor = CastChecked<ASKCollectible>(ObjectInfo->GetObject());
        const float Distance = (Actor->GetActorLocation() - Viewpoint.GetLocation()).Size();

        const float amountDelta = FMath::GetMappedRangeValueClamped(TRange<float>(0.f, 300.f), TRange<float>(1.f, 4.f),
                                                                    (float)CollectiblesAmount);

        const float adjustedMaxDistance = 2500.f / amountDelta;

        if (Distance >= adjustedMaxDistance && BaseMesh->GetComponentVelocity().Size() < 10.f) return 2.f;
        if (Distance >= adjustedMaxDistance) return 1.f;
    }

    return 0.f;
}

void ASKCollectible::PostSignficanceFunction(USignificanceManager::FManagedObjectInfo *ObjectInfo,
                                             float OldSignificance, float Significance, bool bFinal)
{
    if (ObjectInfo->GetTag() == *GetName())
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

    /*GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue,
                                     GetName() + " Significance: " + FString::SanitizeFloat(Significance) +
                                         " total amount: " + FString::FromInt(CollectiblesAmount),
                                     true);*/
}

void ASKCollectible::SetSignificanceMode(const ESignificancePreset Preset)
{

    switch (Preset)
    {
    case ESignificancePreset::High:
        //
        SetActorTickInterval(0.f);

        if (!BaseMesh->RigidBodyIsAwake()) BaseMesh->BodyInstance.WakeInstance();
        BaseMesh->SetCastShadow(true);
        BaseMesh->SetGenerateOverlapEvents(true);

        BaseMesh->BodyInstance.LinearDamping = InitialLinearDampening;
        BaseMesh->BodyInstance.AngularDamping = InitialAngularDampening;
        //
        break;
    case ESignificancePreset::Medium:
        //
        SetActorTickInterval(0.1f);

        BaseMesh->SetCastShadow(false);
        BaseMesh->SetGenerateOverlapEvents(false);

        BaseMesh->BodyInstance.LinearDamping = InitialLinearDampening * 10;
        BaseMesh->BodyInstance.AngularDamping = InitialAngularDampening * 2;
        //
        break;
    case ESignificancePreset::Low:
        //
        SetActorTickInterval(0.25f);

        BaseMesh->SetCastShadow(false);
        BaseMesh->SetGenerateOverlapEvents(false);

        BaseMesh->BodyInstance.LinearDamping = InitialLinearDampening * 10;
        BaseMesh->BodyInstance.AngularDamping = InitialAngularDampening * 2;

        BaseMesh->PutRigidBodyToSleep();
        //
        break;
    default:
        //
        checkNoEntry();
        break;
    }
}
