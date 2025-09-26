// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "S_AISightPerceptionComponent.generated.h"



UCLASS()
class CPP_02_API US_AISightPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:

	AActor* GetNearestEnemy() const;
	
	
};
