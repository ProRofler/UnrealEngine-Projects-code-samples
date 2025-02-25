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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Attributes|Regen Gameplay Effects")
    TSubclassOf<UGameplayEffect> StaminaRegenGameplayEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Attributes|Regen Gameplay Effects")
    TSubclassOf<UGameplayEffect> HealthRegenGameplayEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Attributes|Damage Gameplay Effects")
    TSubclassOf<UGameplayEffect> FallDamageGameplayEffect;

    void ValidateData() const;
};
