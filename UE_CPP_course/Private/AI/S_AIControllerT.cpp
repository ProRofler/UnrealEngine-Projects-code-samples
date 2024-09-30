// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/S_AIControllerT.h"
#include "AI/S_AICharacter.h"
#include "Components/S_AISightPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


AS_AIControllerT::AS_AIControllerT()
{
    SightPerceptionComponent = CreateDefaultSubobject<US_AISightPerceptionComponent>("Sight perception component");
    SetPerceptionComponent(*SightPerceptionComponent);

    bWantsPlayerState = true;

}

void AS_AIControllerT::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto AIChar = Cast<AS_AICharacter>(InPawn);
    if (AIChar)
    {
        RunBehaviorTree(AIChar->GetBehaviorTree());
    }
}

void AS_AIControllerT::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //const auto ActorToAim = SightPerceptionComponent->GetNearestEnemy();
    SetFocus(GetFocusOnActor());

}

AActor* AS_AIControllerT::GetFocusOnActor() const
{
    if(!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnName));

}
