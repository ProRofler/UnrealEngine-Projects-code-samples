// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "S_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CPP_02_API AS_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<AActor*> Pawns;

	int32 PawnIndex = 1;

	void NextPawn();
	void PreviousPawn();
	
};
