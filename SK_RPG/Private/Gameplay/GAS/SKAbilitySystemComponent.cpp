// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Core/SKLogCategories.h"
#include "Gameplay/GAS/Abilities/SKAbilityBase.h"
#include "Logging/StructuredLog.h"
#include "Utils/DataAssets/SKAbilitiesDataAsset.h"
#include "Utils/DataAssets/SKBasicGameplayEffectsDataAsset.h"

void USKAbilitySystemComponent::BeginPlay()
{
    Super::BeginPlay();

    InitAbilities();
    ValidateGEDataAssets();
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

    StaminaRegenActiveGESpecHandle = ApplyGameplayEffectSpecToSelf(
        *MakeGESpecHandle(BasicGameplayEffectsDataAsset->StaminaRegenGameplayEffect).Data.Get());
    check(StaminaRegenActiveGESpecHandle.IsValid());

    UE_LOGFMT(LogSKAbilitySystem, Display, "{Owner} succesfully applied {Effect}", ("Owner", GetOwner()->GetName()),
              ("Effect", BasicGameplayEffectsDataAsset->StaminaRegenGameplayEffect->GetName()));
}

void USKAbilitySystemComponent::StopStaminaRegeneration()
{
    if (!StaminaRegenActiveGESpecHandle.IsValid()) return;

    RemoveActiveGameplayEffect(StaminaRegenActiveGESpecHandle);
    StaminaRegenActiveGESpecHandle.Invalidate();

    if (!StaminaRegenActiveGESpecHandle.IsValid())
        UE_LOGFMT(LogSKAbilitySystem, Display, "{Owner} succesfully removed {Effect}", ("Owner", GetOwner()->GetName()),
                  ("Effect", BasicGameplayEffectsDataAsset->StaminaRegenGameplayEffect->GetName()));
}

void USKAbilitySystemComponent::StartHealthRegeneration()
{
    if (HealthRegenActiveGESpecHandle.IsValid()) return;

    HealthRegenActiveGESpecHandle = ApplyGameplayEffectSpecToSelf(
        *MakeGESpecHandle(BasicGameplayEffectsDataAsset->HealthRegenGameplayEffect).Data.Get());
    check(HealthRegenActiveGESpecHandle.IsValid());

    UE_LOGFMT(LogSKAbilitySystem, Display, "{Owner} succesfully applied {Effect}", ("Owner", GetOwner()->GetName()),
              ("Effect", BasicGameplayEffectsDataAsset->HealthRegenGameplayEffect->GetName()));
}

void USKAbilitySystemComponent::StopHealthRegeneration()
{
    if (!HealthRegenActiveGESpecHandle.IsValid()) return;

    RemoveActiveGameplayEffect(HealthRegenActiveGESpecHandle);
    HealthRegenActiveGESpecHandle.Invalidate();

    UE_LOGFMT(LogSKAbilitySystem, Display, "{Owner} succesfully removed {Effect}", ("Owner", GetOwner()->GetName()),
              ("Effect", BasicGameplayEffectsDataAsset->HealthRegenGameplayEffect->GetName()));
}

void USKAbilitySystemComponent::ApplyFallDamage(const float &FallSpeed)
{
    auto EffectSpec = MakeGESpecHandle(BasicGameplayEffectsDataAsset->FallDamageGameplayEffect);

    if (-FallSpeed < LandingSpeedRange.X) return;

    EffectSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Character.Data.FallDamage")),
                                             CalculateFallDamage(FallSpeed));
    ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
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

    if (GrantedAbilitiesDataAsset)
    {
        if (!GrantedAbilitiesDataAsset->GrantedAbilities.IsEmpty())
        {
            for (auto &Ability : GrantedAbilitiesDataAsset->GrantedAbilities)
            {
                GiveAbility(FGameplayAbilitySpec(Ability, 1, 0, GetOwner()));
            }
        }
    }
}

void USKAbilitySystemComponent::ValidateGEDataAssets() const
{
    if (BasicGameplayEffectsDataAsset)
    {
        BasicGameplayEffectsDataAsset->ValidateData();
    }
    else
    {
        UE_LOGFMT(LogSKAbilitySystem, Error, "{Owner} has no Gameplay Effects Data Asset!",
                  ("Owner", GetOwner()->GetName()));
        checkNoEntry()
    }
}

const float USKAbilitySystemComponent::CalculateFallDamage(const float &FallSpeed)
{
    return FMath::GetMappedRangeValueClamped(LandingSpeedRange, LandingDamageRange, -FallSpeed) * -1.0f;
}
