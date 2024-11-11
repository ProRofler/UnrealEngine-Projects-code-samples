// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Core/SKCoreTypes.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SKInventoryObjectData.generated.h"

UCLASS()
class SIRKNIGHT_API USKInventoryObjectData : public UObject
{
    GENERATED_BODY()

  public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
    FInventoryItemData InventoryItemData;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
    AActor* ItemData;
};
