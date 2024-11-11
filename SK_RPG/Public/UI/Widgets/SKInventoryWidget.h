// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "SKInventoryWidget.generated.h"

class UListView;
class ASKPlayerCharacter;
class USKInventoryObjectData;

UCLASS()
class SIRKNIGHT_API USKInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

  public:
    virtual void NativeConstruct() override;

    TObjectPtr<UListView> GetListView() { return InventoryList; }

    UFUNCTION(BlueprintCallable, Category = "Inventory widget")
    void HandleInventoryOpen();

    void SetPendingUpdate(const bool value) { bIsPendingUpdate = value; }

    UFUNCTION(BlueprintCallable, Category = "Inventory widget")
    void UpdateInventoryWidget();

  protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UListView> InventoryList;

    UFUNCTION()
    void HandleDropItem(AActor *ItemData, const USKItemListEntry *ListEntry);

    UPROPERTY(BlueprintReadWrite)
    bool bIsPendingUpdate = true;

  private:
    ASKPlayerCharacter *Player;

    ASKPlayerCharacter *GetOwningCharacter();
    void InitDelegates();

    TObjectPtr<USKInventoryObjectData> CreateInventoryObjectDataItem(const AActor *Item);
    void HandleEntryWidgetGenerated(UUserWidget &EntryWidget);
    void HandleEntryWidgetReleased(UUserWidget &EntryWidget);
    UFUNCTION()
    void HandleInventoryChanged() { bIsPendingUpdate = true; }
};
