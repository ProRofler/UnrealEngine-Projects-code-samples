// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SKPlayerHUD.generated.h"

class UUserWidget;

UCLASS()
class SIRKNIGHT_API ASKPlayerHUD : public AHUD
{
    GENERATED_BODY()

  public:
    virtual void DrawHUD() override;

    TObjectPtr<UUserWidget> GetInventoryWidget() { return InventoryWidget; }

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget classes")
    TSubclassOf<UUserWidget> InventoryWidgetClass;
    TObjectPtr<UUserWidget> InventoryWidget;

  private:
    void DrawCrosshair();
};
