// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/S_AISightPerceptionComponent.h"
#include "AI/S_AIControllerT.h"
#include "S_Health_Component.h"
#include "Perception/AISense_Sight.h"


AActor* US_AISightPerceptionComponent::GetNearestEnemy() const
{

    TArray<AActor*> VisibleActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), VisibleActors);
    if (VisibleActors.Num() == 0) return nullptr;

    const auto Controller = Cast<AS_AIControllerT>(GetOwner());
    if(!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float NearestDistance = MAX_FLT;
    AActor* NearestActor = nullptr;

    for (const auto VisibleActor : VisibleActors)
    {
        const auto HealthComponent = VisibleActor->GetComponentByClass<US_Health_Component>();
        if (HealthComponent && !HealthComponent->IsDead())
        {
            const auto ActorDistance = (VisibleActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (ActorDistance<NearestDistance)
            {
                NearestDistance = ActorDistance;
                NearestActor = VisibleActor;
            }
        
        }


    }

    return NearestActor;
}
