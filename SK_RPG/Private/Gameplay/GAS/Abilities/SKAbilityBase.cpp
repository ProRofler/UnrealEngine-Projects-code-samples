// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/Abilities/SKAbilityBase.h"
#include "Characters/SKBaseCharacter.h"
#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

USKAbilityBase::USKAbilityBase() { InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; }

void USKAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    // Get Reference to owning char
    if (const FGameplayAbilityActorInfo *OwnerActorInfo = GetCurrentActorInfo())
    {
        OwnerCharacter = Cast<ASKBaseCharacter>(OwnerActorInfo->AvatarActor);
    }
}

void USKAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo *ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData *TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    }

    if (bEnableLogging)
        UE_LOGFMT(LogSKAbilitySystem, Display, "Ability'{AbilityName}' was activated",
                  ("AbilityName", this->GetName()));

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USKAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                bool bWasCancelled)
{
    if (bEnableLogging)
        UE_LOGFMT(LogSKAbilitySystem, Display, "Ability'{AbilityName}' was deactivated",
                  ("AbilityName", this->GetName()));

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}