// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/SKAttributeSet.h"
#include "Characters/SKBaseCharacter.h"
#include "GameplayEffectExtension.h"

void USKAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
    {
        if (const auto SKCharacter = Cast<ASKBaseCharacter>(Data.Target.GetAvatarActor()))
        {
            const float staminaAmount = GetStamina();
            const auto clampedStamina = FMath::Clamp(staminaAmount, 0.0f, GetMaxStamina());

            SetStamina(clampedStamina);

            SKCharacter->OnStaminaChanged.ExecuteIfBound(Data.EvaluatedData.Magnitude);
        }
    }

    else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        if (const auto SKCharacter = Cast<ASKBaseCharacter>(Data.Target.GetAvatarActor()))
        {
            const float healthAmount = GetHealth();
            const auto clampedHealth = FMath::Clamp(healthAmount, 0.0f, GetMaxHealth());

            SetHealth(clampedHealth);

            SKCharacter->OnHealthChanged.ExecuteIfBound(Data.EvaluatedData.Magnitude);
        }
    }
}
