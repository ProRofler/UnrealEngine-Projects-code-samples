// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "SKAbilitySystemComponent.generated.h"

class USKAbilitiesDataAsset;
class USKBasicGameplayEffectsDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameplayTagChangedSignature, FGameplayTag, Tag, bool, bAdded);
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer & /*AssetTags*/);

UCLASS()
class SIRKNIGHT_API USKAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()
  public:
    friend class ASKBaseCharacter;

    // Fires every time when any gameplay effect applied to self
    FEffectAssetTags EffectAssetTags;

    virtual void BeginPlay() override;

    void AbilityInputTagPressed(const FGameplayTag &InputTag);
    void AbilityInputTagHeld(const FGameplayTag &InputTag);
    void AbilityInputTagReleased(const FGameplayTag &InputTag);

    void HandleEffectApplied(UAbilitySystemComponent *AbilitySystemComponent, const FGameplayEffectSpec &EffectSpec,
                             FActiveGameplayEffectHandle ActiveEffectHandle);

    UFUNCTION(BlueprintCallable, Category = "TagWatcher")
    void WatchTag(FGameplayTag TagToWatch);

    UPROPERTY(BlueprintAssignable, Category = "TagWatcher")
    FGameplayTagChangedSignature OnTagChanged;

    // Checks if the GAS component has a provided tag and adds one if it doesn't
    UFUNCTION(BlueprintCallable, Category = "SKAbilitySystem")
    bool CheckAndAddGameplayTag(const FGameplayTag Tag);
    // Checks if the GAS component has a provided tag and removes one if it does
    UFUNCTION(BlueprintCallable, Category = "SKAbilitySystem")
    bool CheckAndRemoveGameplayTag(const FGameplayTag Tag);

  private:
    void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>> &StartupAbilities);
    void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>> &StartupPassiveAbilities);

    FGameplayEffectSpecHandle MakeGESpecHandle(const TSubclassOf<UGameplayEffect> &GameplayEffectClass);

    void HandleTagChanged(const FGameplayTag Tag, int32 NewCount);
};
