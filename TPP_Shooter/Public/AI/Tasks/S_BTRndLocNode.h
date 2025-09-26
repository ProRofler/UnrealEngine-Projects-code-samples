// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"
#include "S_BTRndLocNode.generated.h"

/**
 *
 */
UCLASS()
class CPP_02_API US_BTRndLocNode : public UBTTaskNode
{
    GENERATED_BODY()

  public:
    US_BTRndLocNode();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float SearchRadius = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector AimLocationKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    bool IsSelfCentered = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "!IsSelfCentered"))
    FBlackboardKeySelector CenterActorKey;
};
