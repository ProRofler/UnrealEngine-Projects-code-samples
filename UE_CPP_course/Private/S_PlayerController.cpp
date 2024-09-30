// Fill out your copyright notice in the Description page of Project Settings.


#include "S_PlayerController.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "S_TutorialPawn.h"


DEFINE_LOG_CATEGORY_STATIC(LogControllerOperations, All, All)

void AS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("NextPawn", IE_Pressed, this, &AS_PlayerController::NextPawn);
		InputComponent->BindAction("PreviousPawn", IE_Pressed, this, &AS_PlayerController::PreviousPawn);
	}
	

}

void AS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AS_TutorialPawn::StaticClass(), Pawns);
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Yellow, "Pawns amount: " + FString::FromInt(Pawns.Num()), true);
}

void AS_PlayerController::NextPawn()
{
	if (Pawns.Num() <= 1) 
		{
		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, "Pawns returned NULL", true);
		return;
		}

	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, "Next pawn function was called", true);
	AS_TutorialPawn* CurrentPawn = NULL;
	CurrentPawn = Cast<AS_TutorialPawn>(Pawns[PawnIndex]);
	PawnIndex = (PawnIndex + 1) % Pawns.Num();
	
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, "Current pawn index: " + FString::FromInt(PawnIndex), true);

	if (!CurrentPawn) return;
	UE_LOG(LogControllerOperations, Warning, TEXT("Changing to the next pawn"));
	Possess(CurrentPawn);

}

void AS_PlayerController::PreviousPawn()
{
	if (Pawns.Num() <= 1)
	{
		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, "Pawns returned NULL", true);
		return;
	}

	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, "Next pawn function was called", true);
	
	AS_TutorialPawn* CurrentPawn = nullptr;
	CurrentPawn = Cast<AS_TutorialPawn>(Pawns[PawnIndex]);
	--PawnIndex;
	if (PawnIndex < 0) PawnIndex = Pawns.Num() - 1;

	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, "Current pawn index: " + FString::FromInt(PawnIndex), true);

	if (!CurrentPawn) return;
	UE_LOG(LogControllerOperations, Warning, TEXT("Changing to the next pawn"));
	Possess(CurrentPawn);


}
