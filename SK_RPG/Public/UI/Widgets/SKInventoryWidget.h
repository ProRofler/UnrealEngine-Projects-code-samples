// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/SKUserWidgetBase.h"

#include "SKInventoryWidget.generated.h"

class UListView;
class ASKPlayerCharacter;
class USKInventoryObjectData;

UCLASS()
class SIRKNIGHT_API USKInventoryWidget : public USKUserWidgetBase
{
    GENERATED_BODY()

  public:
    virtual void NativeConstruct() override;

    TObjectPtr<UListView> GetListView() { return InventoryList; }

    UFUNCTION(BlueprintCallable, Category = "Inventory widget")
    void HandleInventoryOpen();

    UFUNCTION(BlueprintCallable, Category = "Inventory widget")
    void UpdateInventoryWidget();

  protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UListView> InventoryList;

    UFUNCTION()
    void HandleDropItem(const USKItemListEntry *ListEntry, const int32 QuantityToDrop);

    UFUNCTION()
    void HandleUseItem(const USKItemListEntry *ListEntry);

  private:
    bool bIsPendingUpdate = true;

    void InitDelegates();

    void HandleEntryWidgetGenerated(UUserWidget &EntryWidget);
    void HandleEntryWidgetReleased(UUserWidget &EntryWidget);

    UFUNCTION()
    void HandleInventoryListUpdate();
};
