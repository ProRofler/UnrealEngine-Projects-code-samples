// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Core/EnhancedInputData.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"

#include "SKPlayerController.generated.h"

class ASKPlayerHUD;
class ASKPlayerCharacter;
class USKAbilitySystemComponent;
class USKAbilityInputConfigDataAsset;

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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    FEnhancedInputData InputData;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<USKAbilityInputConfigDataAsset> InputConfig;

    virtual void OnPossess(APawn *aPawn) override;

  private:
    TWeakObjectPtr<ASKPlayerHUD> PlayerHUD;
    TWeakObjectPtr<ASKPlayerCharacter> SKPlayerCharacter;
    TWeakObjectPtr<USKAbilitySystemComponent> SKAbilitySystemComponent;

    void ControllerSetup();
    void InitializeComponents();

    void AbilityInputTagPressed(FGameplayTag InputTag);
    void AbilityInputTagReleased(FGameplayTag InputTag);
    void AbilityInputTagHeld(FGameplayTag InputTag);
};
