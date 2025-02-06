// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Core/SKLogCategories.h"
#include "Gameplay/GAS/Abilities/SKAbilityBase.h"
#include "Logging/StructuredLog.h"

void USKAbilitySystemComponent::InitializeComponent()
{
    Super::InitializeComponent();

    InitAbilities();

    if (!StaminaRegenGEClass)
    {
        UE_LOGFMT(LogSKAbilitySystem, Error, "{Owner} has no stamina regeneration Gameplay Effect set!",
                  ("Owner", GetOwner()->GetName()));
        checkNoEntry()
    }
}

bool USKAbilitySystemComponent::IsAbilityActive(TSubclassOf<UGameplayAbility> AbilityClass) const
{
    if (!AbilityClass)
    {
        return false;
    }

    for (const FGameplayAbilitySpec &Spec : GetActivatableAbilities())
    {
        if (Spec.Ability && Spec.Ability->GetClass() == AbilityClass)
        {
            if (Spec.IsActive())
            {
                return true;
            }
        }
    }

    return false;
}

bool USKAbilitySystemComponent::CancelAbilityByClass(
    TSubclassOf<UGameplayAbility> AbilityClass) // returns true if the ability was succesfully canceled
{

    for (const FGameplayAbilitySpec &Spec : GetActivatableAbilities())
    {
        if (Spec.Ability && Spec.Ability->GetClass() == AbilityClass)
        {
            if (Spec.IsActive())
            {
                CancelAbility(Spec.Ability);
                return true;
            }
        }
    }

    return false;
}

bool USKAbilitySystemComponent::CheckAndAddGameplayTag(const FGameplayTag &Tag)
{
    if (HasMatchingGameplayTag(Tag))
    {
        return false;
    }
    else
    {
        AddLooseGameplayTag(Tag);
        return true;
    }
}

bool USKAbilitySystemComponent::CheckAndRemoveGameplayTag(const FGameplayTag &Tag)
{
    if (HasMatchingGameplayTag(Tag))
    {
        RemoveLooseGameplayTag(Tag);
        return true;
    }
    else
    {
        return false;
    }
}

FGameplayEffectSpecHandle USKAbilitySystemComponent::MakeGESpecHandle(
    const TSubclassOf<UGameplayEffect> &GameplayEffectClass)
{
    auto geContext = MakeEffectContext();
    geContext.AddSourceObject(GetOwner());

    const auto gERegenSpec = MakeOutgoingSpec(GameplayEffectClass, 1.0f, geContext);
    check(gERegenSpec.IsValid());

    return gERegenSpec;
}

void USKAbilitySystemComponent::StartStaminaRegeneration()
{
    if (StaminaRegenActiveGESpecHandle.IsValid()) return;

    StaminaRegenActiveGESpecHandle = ApplyGameplayEffectSpecToSelf(*MakeGESpecHandle(StaminaRegenGEClass).Data.Get());
    check(StaminaRegenActiveGESpecHandle.IsValid());

    UE_LOGFMT(LogSKAbilitySystem, Display, "{Owner} succesfully applied {Effect}", ("Owner", GetOwner()->GetName()),
              ("Effect", StaminaRegenGEClass->GetName()));
}

void USKAbilitySystemComponent::StopStaminaRegeneration()
{
    if (!StaminaRegenActiveGESpecHandle.IsValid()) return;

    RemoveActiveGameplayEffect(StaminaRegenActiveGESpecHandle);
    StaminaRegenActiveGESpecHandle.Invalidate();

    if (!StaminaRegenActiveGESpecHandle.IsValid())
        UE_LOGFMT(LogSKAbilitySystem, Display, "{Owner} succesfully removed {Effect}", ("Owner", GetOwner()->GetName()),
                  ("Effect", StaminaRegenGEClass->GetName()));
}

void USKAbilitySystemComponent::InitAbilities()
{
    InitAbilityActorInfo(GetOwner(), GetOwner());

    GiveAbility(FGameplayAbilitySpec(USKAbilityBase::StaticClass(), 1, 0));

    if (SprintingAbility)
    {
        GiveAbility(FGameplayAbilitySpec(SprintingAbility, 1, 0));
    }
    else
    {
        SprintingAbility = USKAbilityBase::StaticClass();
        UE_LOGFMT(
            LogSKAbilitySystem, Warning,
            "Actor '{ActorName}' sprinting ability was not assigned! Basic ability was assigned automatically instead.",
            ("ActorName", GetOwner()->GetName()));
    }

    if (InteractAbility)
    {
        GiveAbility(FGameplayAbilitySpec(InteractAbility, 1, 0));
    }
    else
    {
        SprintingAbility = USKAbilityBase::StaticClass();
        UE_LOGFMT(
            LogSKAbilitySystem, Warning,
            "Actor '{ActorName}' interact ability was not assigned! Basic ability was assigned automatically instead.",
            ("ActorName", GetOwner()->GetName()));
    }

    if (!GrantedAbilities.IsEmpty())
    {
        for (auto &Ability : GrantedAbilities)
        {
            GiveAbility(FGameplayAbilitySpec(Ability, 1, 0, GetOwner()));
        }
    }
}
