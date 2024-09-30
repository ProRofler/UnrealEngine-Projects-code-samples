// Fill out your copyright notice in the Description page of Project Settings.


#include "S_TutorialGameMode.h"
#include "S_TutorialPawn.h"
#include "S_PlayerController.h"
#include "UI/S_TempHUD.h"
#include "GameFramework/DefaultPawn.h" // remove this after creating our own pawn

AS_TutorialGameMode::AS_TutorialGameMode()
{

	//change that when custom pawn is ready
	DefaultPawnClass = AS_TutorialPawn::StaticClass();
	PlayerControllerClass = AS_PlayerController::StaticClass();
	HUDClass = AS_TempHUD::StaticClass();

}