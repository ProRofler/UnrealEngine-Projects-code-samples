// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/S_BTLowHealthDecorator.h"
#include "AIController.h"
#include "S_Health_Component.h"

US_BTLowHealthDecorator::US_BTLowHealthDecorator()
{
    NodeName = "Health decorator";
}

bool US_BTLowHealthDecorator::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
        return false;
    // UE_LOG(LogTemp, Display, TEXT("GOT controller"));
    const auto HealthComponent = Controller->GetPawn()->GetComponentByClass<US_Health_Component>();
    if (!HealthComponent || HealthComponent->IsDead())
        return false;
    // UE_LOG(LogTemp, Display, TEXT("GOT component"));

    return HealthThreshold > HealthComponent->GetHealth();
}
