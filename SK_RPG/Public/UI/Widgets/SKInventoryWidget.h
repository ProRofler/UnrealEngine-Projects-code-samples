// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/SKUserWidgetBase.h"

#include "SKInventoryWidget.generated.h"

class ASKPlayerCharacter;
class USKInventoryObjectData;
class USKListViewWidget;

UCLASS()
class SIRKNIGHT_API USKInventoryWidget : public USKUserWidgetBase
{

    GENERATED_BODY()

  public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, Category = "Inventory widget")
    void HandleInventoryOpen();

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USKListViewWidget> WeaponsListViewWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USKListViewWidget> KeysListViewWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USKListViewWidget> MiscListViewWidget;

  private:
    bool bIsPendingUpdate = true;

    void InitDelegates();

    UFUNCTION()
    void HandleInventoryListUpdate();
};
