// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SKPlayerHUD.generated.h"

class UUserWidget;
class USKInventoryWidget;
class USKMainAttributesWidget;
class ASKPlayerCharacter;

UCLASS()
class SIRKNIGHT_API ASKPlayerHUD : public AHUD
{
    GENERATED_BODY()

  public:
    virtual void DrawHUD() override;

    TObjectPtr<USKInventoryWidget> &GetInventoryWidget() { return InventoryWidget; }
    TObjectPtr<USKMainAttributesWidget> &GetMainAttributesWidget() { return MainAttributesWidget; }

    bool IsInventoryOpen() { return bIsInventoryOpen; }
    void ToggleInventoryVisibility();

    UFUNCTION(BlueprintPure, Category = "SK Getters")
    ASKPlayerCharacter *GetSKPlayerCharacter() { return SKPlayer.Get(); };

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget classes")
    TSubclassOf<USKInventoryWidget> InventoryWidgetClass;
    TObjectPtr<USKInventoryWidget> InventoryWidget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget classes")
    TSubclassOf<USKMainAttributesWidget> MainAttributesWidgettClass;
    TObjectPtr<USKMainAttributesWidget> MainAttributesWidget;

    UPROPERTY(BlueprintReadWrite, Category = "UI")
    bool bIsInventoryOpen = false;

  private:
    void DrawCrosshair();

    TWeakObjectPtr<ASKPlayerCharacter> SKPlayer = nullptr;
};
