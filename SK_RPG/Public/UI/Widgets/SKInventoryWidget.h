// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Core/SKCoreTypes.h"
#include "CoreMinimal.h"
#include "SKInventoryWidget.generated.h"

class UListView;

UCLASS()
class SIRKNIGHT_API USKInventoryWidget : public UUserWidget
{
    GENERATED_BODY()
  public:
    virtual void NativeConstruct() override;

    void UpdateInventoryList();
    void AddToInventoryList(FInventoryItemData& Data);

  protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UListView> InventoryList;
};
