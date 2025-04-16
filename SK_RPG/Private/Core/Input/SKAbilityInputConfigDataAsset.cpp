// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Core/Input/SKAbilityInputConfigDataAsset.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

const UInputAction *USKAbilityInputConfigDataAsset::FindAbilityInputActionForTag(const FGameplayTag &InputTag,
                                                                                 bool bLogNotFound) const
{
    for (const auto &Action : AbilityInputActions)
    {
        if (Action.InputAction && Action.InputTag == InputTag)
        {
            return Action.InputAction;
        }
    }

    if (bLogNotFound)
    {
        UE_LOGFMT(LogSKInput, Error, "Can't find AbilityInputAction for InputTag {1}, on InputConfig {2}",
                  *InputTag.ToString(), *GetNameSafe(this));
    }

    return nullptr;
}
