// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/S_BTRndLocNode.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

US_BTRndLocNode::US_BTRndLocNode()
{
    NodeName = "Next location";
}

EBTNodeResult::Type US_BTRndLocNode::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    const auto AIController = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!AIController || !Blackboard)
        return EBTNodeResult::Failed;

    const auto AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return EBTNodeResult::Failed;

    const auto NavSys = UNavigationSystemV1::GetNavigationSystem(AIPawn);
    if (!NavSys)
        return EBTNodeResult::Failed;

    FNavLocation NavLoc;
    auto Location = AIPawn->GetActorLocation();

    if (!IsSelfCentered)
    {
        const auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor)
            return EBTNodeResult::Failed;

        Location = CenterActor->GetActorLocation();
    }

    const auto NewLoc = NavSys->GetRandomReachablePointInRadius(Location, SearchRadius, NavLoc);
    if (!NewLoc)
        return EBTNodeResult::Failed;

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLoc.Location);
    return EBTNodeResult::Succeeded;
}
