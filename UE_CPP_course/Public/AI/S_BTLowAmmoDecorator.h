// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "S_BTLowAmmoDecorator.generated.h"

class AS_BaseWeapon;

UCLASS()
class CPP_02_API US_BTLowAmmoDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	US_BTLowAmmoDecorator();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<AS_BaseWeapon> WeaponAmmoType = nullptr;
};
