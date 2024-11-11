// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SKPlayerController.generated.h"

class ASKPlayerHUD;

UCLASS()
class SIRKNIGHT_API ASKPlayerController : public APlayerController
{
    GENERATED_BODY()

  public:
    virtual void BeginPlay() override;

    void ToggleInventoryHUD();

    TWeakObjectPtr<ASKPlayerHUD> &GetPlayerHUD(){return PlayerHUD;}

  private:
    TWeakObjectPtr<ASKPlayerHUD> PlayerHUD;

    void InitializeComponents();
};
