// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Core/EnhancedInputData.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "SKPlayerController.generated.h"

class ASKPlayerHUD;
class ASKPlayerCharacter;

UCLASS()
class SIRKNIGHT_API ASKPlayerController : public APlayerController
{
    GENERATED_BODY()

  public:
    virtual void BeginPlay() override;

    void ToggleInventoryHUD();

    UFUNCTION(BlueprintCallable, Category = "SK Getters")
    ASKPlayerHUD *GetPlayerHUD() { return PlayerHUD.Get(); }

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "_Enhanced input settings")
    FEnhancedInputData InputData;

    virtual void OnPossess(APawn *aPawn) override;

  private:
    TWeakObjectPtr<ASKPlayerHUD> PlayerHUD;
    TWeakObjectPtr<ASKPlayerCharacter> SKPlayerCharacter;

    void ControllerSetup();
    void InitializeComponents();
};
