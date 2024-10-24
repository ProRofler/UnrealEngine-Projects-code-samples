// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "SKItemListEntry.generated.h"

UCLASS()
class SIRKNIGHT_API USKItemListEntry : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()

  public:
    virtual void NativeOnListItemObjectSet(UObject *ListItemObject) override;

  protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TXT_Item_Name;
};
