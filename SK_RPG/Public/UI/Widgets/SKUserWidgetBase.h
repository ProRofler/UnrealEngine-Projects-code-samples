// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UI/SKPlayerHUD.h"

#include "SKUserWidgetBase.generated.h"

class ASKPlayerHUD;

UCLASS()
class SIRKNIGHT_API USKUserWidgetBase : public UUserWidget
{
    GENERATED_BODY()

  public:
    virtual void NativeConstruct() override;

  protected:
    UFUNCTION(BlueprintPure)
    ASKPlayerHUD *GetSKPlayerHud();

  private:
    TWeakObjectPtr<ASKPlayerHUD> PlayerHUD = nullptr;
};
