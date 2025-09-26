// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"
#include "S_BTChangeWeaponService.generated.h"

UCLASS()
class CPP_02_API US_BTChangeWeaponService : public UBTService
{
    GENERATED_BODY()

  public:
    US_BTChangeWeaponService();

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

  protected:
      UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Probability", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ChangeProbability = 0.6f;


};
