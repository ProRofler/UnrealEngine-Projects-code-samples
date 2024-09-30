// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "S_TempHUD.generated.h"

/**
 *
 */
UCLASS()
class CPP_02_API AS_TempHUD : public AHUD
{
    GENERATED_BODY()

  public:
    virtual void DrawHUD() override;

    void DrawCrosshair();

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerWidgetClass;

    virtual void BeginPlay() override;

private:

    void DrawHud();
};
