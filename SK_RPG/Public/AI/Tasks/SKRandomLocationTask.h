// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "SKRandomLocationTask.generated.h"

UCLASS()
class SIRKNIGHT_API USKRandomLocationTask : public UBTTaskNode
{
    GENERATED_BODY()

  public:
    USKRandomLocationTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float SearchRadius = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector AimLocationKey;
};
