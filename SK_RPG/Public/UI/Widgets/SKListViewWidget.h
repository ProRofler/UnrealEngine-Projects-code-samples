// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/SKUserWidgetBase.h"

#include "Core/SKCoreTypes.h"

#include "SKListViewWidget.generated.h"

class UListView;
class USKItemListEntry;

UCLASS()
class SIRKNIGHT_API USKListViewWidget : public USKUserWidgetBase
{
    GENERATED_BODY()

  public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, Category = "Inventory widget")
    void UpdateListViewWidget();

  protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UListView> InventoryList;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SK List View Setting")
    ECollectibleType ViewCollectibleType = ECollectibleType::Misc;

    UFUNCTION()
    void HandleDropItem(const USKItemListEntry *ListEntry, const int32 QuantityToDrop);

    UFUNCTION()
    void HandleUseItem(const USKItemListEntry *ListEntry);

  private:
    bool bIsPendingUpdate = true;

    void InitDelegates();

    void HandleEntryWidgetGenerated(UUserWidget &EntryWidget);
    void HandleEntryWidgetReleased(UUserWidget &EntryWidget);
};
