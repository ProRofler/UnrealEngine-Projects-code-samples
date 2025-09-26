// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "AI/Tasks/SKRandomLocationTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

USKRandomLocationTask::USKRandomLocationTask() { NodeName = "Next location"; }

EBTNodeResult::Type USKRandomLocationTask::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    const AAIController *AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent *Blackboard = OwnerComp.GetBlackboardComponent();
    if (!AIController || !Blackboard) return EBTNodeResult::Failed;

    APawn *AIPawn = AIController->GetPawn();
    if (!AIPawn) return EBTNodeResult::Failed;

    const UNavigationSystemV1 *NavSys = UNavigationSystemV1::GetNavigationSystem(AIPawn);
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation NavLoc;
    auto Location = AIPawn->GetActorLocation();

#if 0
    if (!IsSelfCentered)
    {
        const auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor) return EBTNodeResult::Failed;

        Location = CenterActor->GetActorLocation();
    }
#endif

    const bool NewLoc = NavSys->GetRandomReachablePointInRadius(Location, SearchRadius, NavLoc);
    if (!NewLoc) return EBTNodeResult::Failed;

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLoc.Location);
    return EBTNodeResult::Succeeded;
}
