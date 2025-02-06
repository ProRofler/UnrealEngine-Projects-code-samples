// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "SKItemListEntry.generated.h"

class USKInventoryObjectData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemDroppedSignature, const USKItemListEntry *, ListEntry, const int32,
                                             QuantityToDrop);

UCLASS()
class SIRKNIGHT_API USKItemListEntry : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()

  public:
    virtual void NativeOnListItemObjectSet(UObject *ListItemObject) override;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnItemDroppedSignature OnItemDropCalled;

    UFUNCTION()
    void UnbindDelegates();

    const TObjectPtr<USKInventoryObjectData> &GetInventoryItemData() const { return InventoryItemData; };

  protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock *TXT_Item_Name;

    UPROPERTY(meta = (BindWidget))
    UTextBlock *TXT_Item_Quantity;

    UPROPERTY(meta = (BindWidget))
    UButton *BTN_Drop_Button;

  private:
    UFUNCTION()
    void HandleItemDrop();

    TObjectPtr<USKInventoryObjectData> InventoryItemData = nullptr;
};
