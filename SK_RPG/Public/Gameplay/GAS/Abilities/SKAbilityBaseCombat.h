// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/Abilities/SKAbilityBase.h"

#include "SKAbilityBaseCombat.generated.h"

UCLASS()
class SIRKNIGHT_API USKAbilityBaseCombat : public USKAbilityBase
{
    GENERATED_BODY()

  protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData *TriggerEventData);

  protected:
    UFUNCTION(BlueprintCallable, Category = "SK Ability|Combat")
    void HandleWeaponTrace();

  private:
    UFUNCTION()
    void OnGameplayEvent(FGameplayEventData Payload);
};
