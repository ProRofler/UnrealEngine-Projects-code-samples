// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "CoreMinimal.h"
#include "S_BTLowHealthDecorator.generated.h"


UCLASS()
class CPP_02_API US_BTLowHealthDecorator : public UBTDecorator
{
    GENERATED_BODY()

  public:
      US_BTLowHealthDecorator();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float HealthThreshold = 40.0f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
