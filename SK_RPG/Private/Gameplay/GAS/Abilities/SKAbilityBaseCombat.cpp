// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/Abilities/SKAbilityBaseCombat.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemInterface.h"
#include "Characters/Components/SKWeaponComponent.h"
#include "Characters/SKBaseCharacter.h"
#include "Core/Interface/SKInterfaceCharacter.h"
#include "Gameplay/GAS/SKNativeGameplayTags.h"

#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

#include "AbilitySystemBlueprintLibrary.h"

void USKAbilityBaseCombat::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo *ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    SetupWeaponTrace();
}

void USKAbilityBaseCombat::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo *ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                      bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (GetSKOwnerCharacter()->GetMesh()->IsSimulatingPhysics())
    {
        ISKInterfaceCharacter::Execute_SetCombatPhysics(GetSKOwnerCharacter(), false);
    }
}

void USKAbilityBaseCombat::SetupWeaponTrace()
{
    UAbilityTask_WaitGameplayEvent *traceStartTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this, FSKGameplayTags::Get().Event_Combat_WeaponTraceStart, nullptr, false, true);

    UAbilityTask_WaitGameplayEvent *traceEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this, FSKGameplayTags::Get().Event_Combat_WeaponTraceEnd, nullptr, false, true);

    UAbilityTask_WaitGameplayEvent *traceHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this, FSKGameplayTags::Get().Event_Combat_Hit, nullptr, false, true);

    if (traceStartTask && traceEndTask && traceHitTask)
        if (traceStartTask && traceEndTask)
        {
            traceStartTask->EventReceived.AddDynamic(this, &USKAbilityBaseCombat::OnGameplayEventTrace);
            traceEndTask->EventReceived.AddDynamic(this, &USKAbilityBaseCombat::OnGameplayEventTrace);
            traceHitTask->EventReceived.AddDynamic(this, &USKAbilityBaseCombat::OnGameplayEventHit);

            traceStartTask->ReadyForActivation();
            traceEndTask->ReadyForActivation();
            traceHitTask->ReadyForActivation();
        }
}

void USKAbilityBaseCombat::OnGameplayEventTrace(FGameplayEventData Payload)
{
    const auto weaponComponent = ISKInterfaceCharacter::Execute_GetWeaponComponent(GetSKOwnerCharacter());
    if (!weaponComponent) return;

    if (Payload.EventTag == FSKGameplayTags::Get().Event_Combat_WeaponTraceStart)
    {
        weaponComponent->SetIsTracingMelee(true);
    }
    else if (Payload.EventTag == FSKGameplayTags::Get().Event_Combat_WeaponTraceEnd)
    {
        weaponComponent->SetIsTracingMelee(false);
    }
}

void USKAbilityBaseCombat::OnGameplayEventHit(FGameplayEventData Payload)
{
    const auto weaponComponent = ISKInterfaceCharacter::Execute_GetWeaponComponent(GetSKOwnerCharacter());
    if (!weaponComponent) return;

    weaponComponent->SetIsTracingMelee(false);

    if (Payload.Target && Payload.Target->Implements<UAbilitySystemInterface>())
    {
        AActor *target = ConstCast(Payload.Target);
        if (!target) return;

        FGameplayEventData tPayload;
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(target, FSKGameplayTags::Get().Event_Combat_Damage,
                                                                 tPayload);

        if (DamageEffect)
        {
            IAbilitySystemInterface *ASI = Cast<IAbilitySystemInterface>(target);
            const auto ASC = ASI->GetAbilitySystemComponent();

            FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffect, 0, ASC->MakeEffectContext());
            ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}