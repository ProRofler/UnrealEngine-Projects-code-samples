// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/S_BTServiceA.h"
#include "AI/S_AIControllerT.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/S_AISightPerceptionComponent.h"


US_BTServiceA::US_BTServiceA()
{
    NodeName = "Find Enemy";
}

void US_BTServiceA::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto BlackBoard = OwnerComp.GetBlackboardComponent();
    if(!BlackBoard) return;

    const auto Controller = OwnerComp.GetOwner();
    const auto PerceptionComponent = Controller->GetComponentByClass<US_AISightPerceptionComponent>();
    if (!PerceptionComponent) return;

    BlackBoard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetNearestEnemy());


    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
