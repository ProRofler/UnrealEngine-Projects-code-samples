// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "SKInventoryDataTable.generated.h"

USTRUCT(BlueprintType)
struct SIRKNIGHT_API FSKInventoryDataTable : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class ASKCollectible> ItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Amount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool MainWeaponSlot = false;
};
