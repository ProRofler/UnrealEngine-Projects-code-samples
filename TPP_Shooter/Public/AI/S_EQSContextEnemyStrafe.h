// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "S_EQSContextEnemyStrafe.generated.h"

/**
 *
 */
UCLASS()
class CPP_02_API US_EQSContextEnemyStrafe : public UEnvQueryContext
{
    GENERATED_BODY()

  public:
    virtual void ProvideContext(FEnvQueryInstance &QueryInstance, FEnvQueryContextData &ContextData) const override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EQS Context")
    FName KeyEnemyActorName = "EnemyActor";
};
