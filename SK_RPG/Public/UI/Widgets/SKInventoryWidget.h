// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "SKInventoryWidget.generated.h"

UCLASS()
class SIRKNIGHT_API USKInventoryWidget : public UUserWidget
{
    GENERATED_BODY()
  public:
    void UpdateInventoryList();
};
