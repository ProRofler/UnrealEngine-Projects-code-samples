// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Core/Input/SKAbilityInputConfigDataAsset.h"
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"

#include "SKInputComponent.generated.h"

UCLASS()
class SIRKNIGHT_API USKInputComponent : public UEnhancedInputComponent
{
    GENERATED_BODY()

  public:
    template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
    void BindAbilityActions(const USKAbilityInputConfigDataAsset *InputConfig, UserClass *Object,
                            PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void USKInputComponent::BindAbilityActions(const USKAbilityInputConfigDataAsset *InputConfig, UserClass *Object,
                                           PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
                                           HeldFuncType HeldFunc)
{
    for (const auto &Action : InputConfig->AbilityInputActions)
    {
        if (Action.InputAction && Action.InputTag.IsValid())
        {
            if (PressedFunc)
            {
                BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
            }

            if (ReleasedFunc)
            {
                BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
            }

            if (HeldFunc)
            {
                BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
            }
        }
    }
}
