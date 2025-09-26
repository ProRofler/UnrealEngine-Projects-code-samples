// Fill out your copyright notice in the Description page of Project Settings.

#include "S_TPP_GameMode.h"
#include "AIController.h"
#include "Player/S_PLayerCharacter.h"
#include "Player/S_TPP_PlayerController.h"

AS_TPP_GameMode::AS_TPP_GameMode()
{
    DefaultPawnClass = AS_PLayerCharacter::StaticClass();
    PlayerControllerClass = AS_TPP_PlayerController::StaticClass();
}

UClass *AS_TPP_GameMode::GetDefaultPawnClassForController_Implementation(AController *InController)
{

    if (InController && InController->IsA<AAIController>())
        return AIPawnClass;

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AS_TPP_GameMode::StartPlay()
{
    Super::StartPlay();

    StartRound();
    SpawnAI();
}

void AS_TPP_GameMode::StartRound()
{

    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &AS_TPP_GameMode::GameTimerUpdate, 1.0f, true);
}

void AS_TPP_GameMode::GameTimerUpdate()
{

    UE_LOG(LogTemp, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {

            UE_LOG(LogTemp, Display, TEXT("************Game over**************"));

            // GameOver();
        }
    }
}

void AS_TPP_GameMode::SpawnAI()
{
    if (!GetWorld())
        return;

    for (int32 i = 0; i < GameData.NumbersOfPlayers - 1; i++)
    {
        FActorSpawnParameters SpawnParam;
        SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnParam);
        RestartPlayer(AIController);
    }
}

void AS_TPP_GameMode::ResetPlayers()
{
    if (!GetWorld())
        return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void AS_TPP_GameMode::ResetOnePlayer(AController *Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    // SetPlayerColor(Controller);
}
