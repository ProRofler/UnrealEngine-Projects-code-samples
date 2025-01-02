// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/Abilities/SKAbilityTest.h"
#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

USKAbilityTest::USKAbilityTest() { InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced; }

void USKAbilityTest::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo *ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData *TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    }

    UE_LOGFMT(LogSKAbilitySystem, Display, "Ability'{AbilityName}' was activated", ("AbilityName", this->GetName()));
}

void USKAbilityTest::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                bool bWasCancelled)
{
    UE_LOGFMT(LogSKAbilitySystem, Display, "Ability'{AbilityName}' was activated", ("AbilityName", this->GetName()));

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
