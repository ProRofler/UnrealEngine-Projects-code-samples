// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"

#include "SKStaminaRegenMMC.generated.h"

UCLASS()
class SIRKNIGHT_API USKStaminaRegenMMC : public UGameplayModMagnitudeCalculation
{
    GENERATED_BODY()

  public:
    virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const override;
};
