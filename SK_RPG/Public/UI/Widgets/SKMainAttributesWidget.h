// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/SKUserWidgetBase.h"

#include "Components/ProgressBar.h"

#include "SKMainAttributesWidget.generated.h"

class ASKPlayerHUD;

UCLASS()
class SIRKNIGHT_API USKMainAttributesWidget : public USKUserWidgetBase
{
    GENERATED_BODY()

  private:
    UPROPERTY(meta = (BindWidget))
    UProgressBar *PB_Stamina;
};
