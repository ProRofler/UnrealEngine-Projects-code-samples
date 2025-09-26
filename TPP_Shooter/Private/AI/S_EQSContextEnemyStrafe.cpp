// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/S_EQSContextEnemyStrafe.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void US_EQSContextEnemyStrafe::ProvideContext(FEnvQueryInstance &QueryInstance, FEnvQueryContextData &ContextData) const
{
    const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());

    const auto BlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
    if (!BlackBoard)
        return;

    const auto ContextActor = BlackBoard->GetValueAsObject(KeyEnemyActorName);
    UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(ContextActor));
}
