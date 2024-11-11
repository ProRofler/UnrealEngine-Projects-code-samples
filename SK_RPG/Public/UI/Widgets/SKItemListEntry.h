// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "SKItemListEntry.generated.h"

class USKInventoryObjectData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemDropped, AActor *, ItemData, const USKItemListEntry *, ListEntry);

UCLASS()
class SIRKNIGHT_API USKItemListEntry : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()

  public:
    virtual void NativeOnListItemObjectSet(UObject *ListItemObject) override;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnItemDropped OnItemDropCalled;

    UFUNCTION()
    void UnbindDelegates();

  protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock *TXT_Item_Name;

    UPROPERTY(meta = (BindWidget))
    UButton *BTN_Drop_Button;

  private:
    AActor *ItemData = nullptr;

    UFUNCTION()
    void HandleItemDrop();
};
