// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SKPlayerHUD.generated.h"

class UUserWidget;
class USKMainAttributesWidget;
class ASKPlayerCharacter;

UCLASS()
class SIRKNIGHT_API ASKPlayerHUD : public AHUD
{
    GENERATED_BODY()

  public:
    // virtual void DrawHUD() override;

    UFUNCTION(BlueprintPure, Category = "SK HUD Getters")
    USKUserWidgetBase *GetPlayerUIWidget() { return PlayerUIWidget; }

    UFUNCTION(BlueprintPure, Category = "SK GUD Getters")
    ASKPlayerCharacter *GetSKPlayerCharacter() { return SKPlayer.Get(); };

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget classes")
    TSubclassOf<USKUserWidgetBase> PlayerUIWidgetClass;
    TObjectPtr<USKUserWidgetBase> PlayerUIWidget;

  private:
    TWeakObjectPtr<ASKPlayerCharacter> SKPlayer = nullptr;

    UFUNCTION()
    void HandlePlayerDeath();
};
