// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "S_BTAimService.generated.h"

/**
 * 
 */
UCLASS()
class CPP_02_API US_BTAimService : public UBTService
{
    GENERATED_BODY()

  public:
    US_BTAimService();

  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
};