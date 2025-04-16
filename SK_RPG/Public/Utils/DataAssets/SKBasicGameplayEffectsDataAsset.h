// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SKBasicGameplayEffectsDataAsset.generated.h"

class UGameplayEffect;

UCLASS()
class SIRKNIGHT_API USKBasicGameplayEffectsDataAsset : public UDataAsset
{
    GENERATED_BODY()

  public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Attributes|Fall damage")
    TSubclassOf<UGameplayEffect> FallDamageGameplayEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Attributes|Fall damage")
    FVector2D LandingSpeedRange = {700.0f, 1200.0f};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Attributes|Fall damage")
    FVector2D LandingDamageRange = {15.0f, 100.0f};

    void ValidateData() const;
};
