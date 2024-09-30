// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/S_BTChangeWeaponService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Weapons/S_PlayerWeaponComponent.h"
#include "AIController.h"
#include "AI/S_AIWeaponComponent.h"

US_BTChangeWeaponService::US_BTChangeWeaponService()
{
        NodeName = "Change weapon";
}

void US_BTChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    //UE_LOG(LogTemp, Display, TEXT("Called for weapon change"));
    
    //const auto BlackBoard = OwnerComp.GetBlackboardComponent();
    //if(!BlackBoard) return;

    const auto Controller = OwnerComp.GetAIOwner();
    if(!Controller) return;
    const auto WeaponComponent = Controller->GetPawn()->GetComponentByClass<US_AIWeaponComponent>();
    if(!WeaponComponent) return;
    
    if (FMath::RandRange(0.01f, 0.99f) < ChangeProbability && !WeaponComponent->IsSecondaryAmmoEmpty())
    {
        WeaponComponent->SwitchWeapon();
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
