// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/S_BTAimService.h"
#include "S_BTAimService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Weapons/S_PlayerWeaponComponent.h"
#include "AIController.h"

US_BTAimService::US_BTAimService()
{

    NodeName = "Shoot Enemy";
}

void US_BTAimService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto BlackBoard = OwnerComp.GetBlackboardComponent();
    if(!BlackBoard) return;
    const auto Controller = OwnerComp.GetAIOwner();
    if(!Controller) return;

    const auto HasAim = BlackBoard && BlackBoard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    const auto WeaponComponent = Controller->GetPawn()->GetComponentByClass<US_PlayerWeaponComponent>();
    if(!WeaponComponent) return;

    HasAim ? WeaponComponent->Fire() : WeaponComponent->StopFire();



    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}