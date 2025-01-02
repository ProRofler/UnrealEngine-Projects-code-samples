// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Core/SKLogCategories.h"
#include "Gameplay/GAS/Abilities/SKAbilityBase.h"
#include "Logging/StructuredLog.h"

void USKAbilitySystemComponent::InitializeComponent()
{
    Super::InitializeComponent();

    InitAbilities();
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

    // Adding all the additional abilities
    if (GrantedAbilities.IsEmpty())
    {
        for (auto Ability : GrantedAbilities)
        {
            GiveAbility(FGameplayAbilitySpec(Ability->GetClass(), 1, 0, this));
        }
    }
}
