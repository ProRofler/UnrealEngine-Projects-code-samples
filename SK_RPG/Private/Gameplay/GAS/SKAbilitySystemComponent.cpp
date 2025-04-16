// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Core/SKLogCategories.h"
#include "Gameplay/GAS/Abilities/SKAbilityBase.h"
#include "Logging/StructuredLog.h"
#include "Utils/DataAssets/SKBasicGameplayEffectsDataAsset.h"

void USKAbilitySystemComponent::BeginPlay()
{
    Super::BeginPlay();
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &USKAbilitySystemComponent::HandleEffectApplied);
}

void USKAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag &InputTag)
{
    if (!InputTag.IsValid()) return;

    //UE_LOGFMT(LogSKInput, Display, "Received pressed {1}", InputTag.ToString());

    FScopedAbilityListLock ActiveScopeLoc(*this);
    for (FGameplayAbilitySpec &AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec);
            if (AbilitySpec.IsActive())
            {
                InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle,
                                      AbilitySpec.ActivationInfo.GetActivationPredictionKey());
            }
        }
    }
}

void USKAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag &InputTag)
{
    if (!InputTag.IsValid()) return;

    //UE_LOGFMT(LogSKInput, Display, "Received held {1}", InputTag.ToString());

    FScopedAbilityListLock ActiveScopeLoc(*this);
    for (FGameplayAbilitySpec &AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec);
            if (!AbilitySpec.IsActive())
            {
                TryActivateAbility(AbilitySpec.Handle);
            }
        }
    }
}

void USKAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag &InputTag)
{
    if (!InputTag.IsValid()) return;

    //UE_LOGFMT(LogSKInput, Display, "Received released {1}", InputTag.ToString());

    FScopedAbilityListLock ActiveScopeLoc(*this);
    for (FGameplayAbilitySpec &AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
        {
            AbilitySpecInputReleased(AbilitySpec);
            InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle,
                                  AbilitySpec.ActivationInfo.GetActivationPredictionKey());
        }
    }
}

void USKAbilitySystemComponent::HandleEffectApplied(UAbilitySystemComponent *AbilitySystemComponent,
                                                    const FGameplayEffectSpec &EffectSpec,
                                                    FActiveGameplayEffectHandle ActiveEffectHandle)
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);

    EffectAssetTags.Broadcast(TagContainer);
}

void USKAbilitySystemComponent::WatchTag(FGameplayTag TagToWatch)
{
    RegisterGameplayTagEvent(TagToWatch, EGameplayTagEventType::NewOrRemoved)
        .AddUObject(this, &USKAbilitySystemComponent::HandleTagChanged);
}

void USKAbilitySystemComponent::HandleTagChanged(const FGameplayTag Tag, int32 NewCount)
{
    const bool bAdded = NewCount > 0;
    OnTagChanged.Broadcast(Tag, bAdded);
}

//

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

//

FGameplayEffectSpecHandle USKAbilitySystemComponent::MakeGESpecHandle(
    const TSubclassOf<UGameplayEffect> &GameplayEffectClass)
{
    auto geContext = MakeEffectContext();
    geContext.AddSourceObject(GetOwner());

    const auto gERegenSpec = MakeOutgoingSpec(GameplayEffectClass, 1.0f, geContext);
    check(gERegenSpec.IsValid());

    return gERegenSpec;
}

//

void USKAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>> &StartupAbilities)
{
    for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        if (const auto *SKAbility = Cast<USKAbilityBase>(AbilitySpec.Ability))
        {
            AbilitySpec.DynamicAbilityTags.AddTag(SKAbility->StartupInputTag);
            GiveAbility(AbilitySpec);
        }
    }
}

void USKAbilitySystemComponent::AddCharacterPassiveAbilities(
    const TArray<TSubclassOf<UGameplayAbility>> &StartupPassiveAbilities)
{
    for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        if (const auto *SKAbility = Cast<USKAbilityBase>(AbilitySpec.Ability))
        {
            GiveAbilityAndActivateOnce(AbilitySpec);
        }
    }
}