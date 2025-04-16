// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/SKAttributeSet.h"
#include "Characters/SKBaseCharacter.h"
#include "Gameplay/GAS/SKNativeGameplayTags.h"
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

            FSKAttributeChangeData StaminaChageData;
            StaminaChageData.ChangedAmount = Data.EvaluatedData.Magnitude;
            StaminaChageData.CurrentValue = GetStamina();
            StaminaChageData.MaxValue = GetMaxStamina();
            StaminaChageData.AttributeTag = FSKGameplayTags::Get().Attibute_Main_Stamina;
            StaminaChageData.EventIncreasedTag = FSKGameplayTags::Get().Event_Attibute_Increase_Stamina;
            StaminaChageData.EventDecreasedTag = FSKGameplayTags::Get().Event_Attibute_Decrease_Stamina;
            StaminaChageData.EventDepletedTag = FSKGameplayTags::Get().Event_Attibute_Depleted_Stamina;
            StaminaChageData.EventFullTag = FSKGameplayTags::Get().Event_Attibute_Full_Stamina;

            SKCharacter->OnMainAttributeChanged.ExecuteIfBound(StaminaChageData);
        }
    }

    else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        if (const auto SKCharacter = Cast<ASKBaseCharacter>(Data.Target.GetAvatarActor()))
        {
            const float healthAmount = GetHealth();
            const auto clampedHealth = FMath::Clamp(healthAmount, 0.0f, GetMaxHealth());

            SetHealth(clampedHealth);

            FSKAttributeChangeData HeathChangeData;
            HeathChangeData.ChangedAmount = Data.EvaluatedData.Magnitude;
            HeathChangeData.CurrentValue = GetHealth();
            HeathChangeData.MaxValue = GetMaxHealth();
            HeathChangeData.AttributeTag = FSKGameplayTags::Get().Attibute_Main_Health;
            HeathChangeData.EventIncreasedTag = FSKGameplayTags::Get().Event_Attibute_Increase_Health;
            HeathChangeData.EventDecreasedTag = FSKGameplayTags::Get().Event_Attibute_Decrease_Health;
            HeathChangeData.EventDepletedTag = FSKGameplayTags::Get().Event_Attibute_Depleted_Health;
            HeathChangeData.EventFullTag = FSKGameplayTags::Get().Event_Attibute_Full_Health;

            SKCharacter->OnMainAttributeChanged.ExecuteIfBound(HeathChangeData);

            SKCharacter->OnMainAttributeChanged.ExecuteIfBound(HeathChangeData);
        }
    }
}
