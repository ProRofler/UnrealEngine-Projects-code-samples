// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "S_TPP_GameMode.generated.h"

class AAIController;

USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamemod settings",
              meta = (ClampMin = "1", ClampMax = "100"))
    int32 NumbersOfPlayers = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamemod settings",
              meta = (ClampMin = "3", ClampMax = "300"))
    int32 RoundTime = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamemod settings", meta = (ClampMin = "1", ClampMax = "10"))
    int32 RoundsNum = 4;
};

UCLASS() class CPP_02_API AS_TPP_GameMode : public AGameModeBase
{
    GENERATED_BODY()

    AS_TPP_GameMode();

    virtual UClass *GetDefaultPawnClassForController_Implementation(AController *InController) override;

  public:
    virtual void StartPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gamemod settings")
    TSubclassOf<AAIController> AIControllerClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gamemod settings")
    TSubclassOf<APawn> AIPawnClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gamemod settings")
    FGameData GameData;

  private:
    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;

    void StartRound();
    void GameTimerUpdate();

    void SpawnAI();
    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

};
