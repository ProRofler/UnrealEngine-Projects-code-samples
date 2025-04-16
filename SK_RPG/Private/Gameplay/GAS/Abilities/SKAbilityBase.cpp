// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/Abilities/SKAbilityBase.h"
#include "Characters/SKBaseCharacter.h"
#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

USKAbilityBase::USKAbilityBase() { InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; }

void USKAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    if (GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
    {
        checkf(false, TEXT("This ability can't be non-instanced!"));
    }
    else if (GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::InstancedPerActor)
    {
        OwnerCharacter = CastChecked<ASKBaseCharacter>(GetAvatarActorFromActorInfo());
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

    if (GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::InstancedPerExecution)
    {
        OwnerCharacter = CastChecked<ASKBaseCharacter>(GetAvatarActorFromActorInfo());
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