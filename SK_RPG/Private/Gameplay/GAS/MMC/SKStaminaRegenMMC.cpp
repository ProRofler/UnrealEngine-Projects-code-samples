// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/MMC/SKStaminaRegenMMC.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Core/SKLogCategories.h"
#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Gameplay/GAS/SKNativeGameplayTags.h"
#include "GameplayEffectTypes.h"
#include "Logging/StructuredLog.h"

float USKStaminaRegenMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const
{

    auto *SourceObj = Spec.GetContext().GetOriginalInstigator();
    if (!SourceObj)
    {
        UE_LOGFMT(LogSKAbilitySystem, Error,
                  "Stamina regen mod magnitude calculation class couldn't get original instigator!");
        return 0.0f;
    }

    const USKAbilitySystemComponent *ASC =
        Cast<USKAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceObj));
    if (!ASC)
    {
        UE_LOGFMT(LogSKAbilitySystem, Error,
                  "Stamina regen mod magnitude calculation class couldn't get Ability system component!");

        return 0.0f;
    }

    float BaseRegen = 1.f;

    if (ASC->HasMatchingGameplayTag(FSKGameplayTags::Get().Character_State_Movement_Idling))
    {
        BaseRegen *= 2.f;
    }
    else if (ASC->HasMatchingGameplayTag(FSKGameplayTags::Get().Character_State_Movement_Walking))
    {
        BaseRegen *= 1.5f;
    }

    return BaseRegen;
}
