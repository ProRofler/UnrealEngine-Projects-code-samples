// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/Abilities/SKAbilityBaseCombat.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemInterface.h"
#include "Characters/Components/SKWeaponComponent.h"
#include "Characters/SKBaseCharacter.h"
#include "Core/Interface/SKInterfaceCharacter.h"
#include "Gameplay/GAS/SKNativeGameplayTags.h"

#include "AbilitySystemBlueprintLibrary.h"

void USKAbilityBaseCombat::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo *ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USKAbilityBaseCombat::HandleWeaponTrace()
{
    UAbilityTask_WaitGameplayEvent *traceStartTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this, FSKGameplayTags::Get().Event_Combat_WeaponTraceStart, nullptr, false, true);

    UAbilityTask_WaitGameplayEvent *traceEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this, FSKGameplayTags::Get().Event_Combat_WeaponTraceEnd, nullptr, false, true);

    UAbilityTask_WaitGameplayEvent *traceHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this, FSKGameplayTags::Get().Event_Combat_Hit, nullptr, true, true);

    if (traceStartTask && traceEndTask && traceHitTask)
    {
        traceStartTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEvent);
        traceEndTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEvent);
        traceHitTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEvent);

        traceStartTask->ReadyForActivation();
        traceEndTask->ReadyForActivation();
        traceHitTask->ReadyForActivation();
    }
}

void USKAbilityBaseCombat::OnGameplayEvent(FGameplayEventData Payload)
{
    const auto weaponComponent = ISKInterfaceCharacter::Execute_GetWeaponComponent(GetSKOwnerCharacter());
    if (!weaponComponent) return;

    const auto EventTag = Payload.EventTag;

    if (EventTag == FSKGameplayTags::Get().Event_Combat_WeaponTraceStart)
    {
        UE_LOG(LogTemp, Warning, TEXT("Trace start"));
        weaponComponent->SetIsTracingSword(true);
    }
    else if (EventTag == FSKGameplayTags::Get().Event_Combat_WeaponTraceEnd)
    {
        UE_LOG(LogTemp, Warning, TEXT("Trace end"));
        weaponComponent->SetIsTracingSword(false);
    }
    else if (EventTag == FSKGameplayTags::Get().Event_Combat_Hit)
    {
        UE_LOG(LogTemp, Warning, TEXT("HIT"));
    }
}
