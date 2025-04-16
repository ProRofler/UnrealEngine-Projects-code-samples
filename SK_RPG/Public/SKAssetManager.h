// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SKAssetManager.generated.h"

UCLASS()
class SIRKNIGHT_API USKAssetManager : public UAssetManager
{
    GENERATED_BODY()
  public:
    static USKAssetManager &Get();

  protected:
    virtual void StartInitialLoading() override;

};
