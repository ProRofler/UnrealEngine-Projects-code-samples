// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "SKMaterialsPrimaryDataAsset.generated.h"

UCLASS()
class SIRKNIGHT_API USKMaterialsPrimaryDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

  public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UMaterialInterface> OutlineMaterial;
};
