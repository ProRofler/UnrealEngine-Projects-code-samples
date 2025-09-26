// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "MyEnvQueryTest_AmmoTakeCheck.generated.h"

/**
 *
 */
UCLASS()
class CPP_02_API UMyEnvQueryTest_AmmoTakeCheck : public UEnvQueryTest
{
    GENERATED_BODY()

  public:
    UMyEnvQueryTest_AmmoTakeCheck(const FObjectInitializer& ObjectInitializer);

    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
