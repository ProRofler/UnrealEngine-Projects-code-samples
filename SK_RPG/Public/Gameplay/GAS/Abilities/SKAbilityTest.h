// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "SKAbilityTest.generated.h"

UCLASS()
class SIRKNIGHT_API USKAbilityTest : public UGameplayAbility
{
    GENERATED_BODY()

  public:
    USKAbilityTest();

  protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData *TriggerEventData);

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                            bool bWasCancelled);
};
