// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "SKAbilitySystemComponent.generated.h"

class USKAbilitiesDataAsset;
class USKBasicGameplayEffectsDataAsset;

UCLASS()
class SIRKNIGHT_API USKAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()
  public:
    friend class ASKBaseCharacter;

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    bool IsAbilityActive(TSubclassOf<UGameplayAbility> AbilityClass) const;
    UFUNCTION(BlueprintCallable)
    bool CancelAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass);

    // Checks if the GAS component has a provided tag and adds one if it doesn't
    UFUNCTION(BlueprintCallable)
    bool CheckAndAddGameplayTag(const FGameplayTag &Tag);

    // Checks if the GAS component has a provided tag and removes one if it does
    UFUNCTION(BlueprintCallable)
    bool CheckAndRemoveGameplayTag(const FGameplayTag &Tag);

    UFUNCTION(BlueprintCallable, Category = "SK Abilities")
    TArray<TSubclassOf<UGameplayAbility>> &GetGrantedAbilities() { return GrantedAbilities; };

    UFUNCTION(BlueprintCallable, Category = "SK Attributes")
    void StartStaminaRegeneration();
    UFUNCTION(BlueprintCallable, Category = "SK Attributes")
    void StopStaminaRegeneration();

    UFUNCTION(BlueprintCallable, Category = "SK Attributes")
    void StartHealthRegeneration();
    UFUNCTION(BlueprintCallable, Category = "SK Attributes")
    void StopHealthRegeneration();

    UFUNCTION(BlueprintCallable, Category = "SK Attributes")
    void ApplyFallDamage(const float &FallSpeed);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Tweaks|Stamina regeneration settings",
              meta = (ClampMin = "0.01", ClampMax = "100.0", UIMin = "0.01", UIMax = "1.5"))
    float StaminaRegenAmount = 0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Tweaks|Stamina regeneration settings",
              meta = (ClampMin = "1.0", ClampMax = "100.0", UIMin = "1.0", UIMax = "1.5"))
    float StamineRegenWalkMultiplier = 1.3f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Tweaks|Stamina regeneration settings",
              meta = (ClampMin = "1.0", ClampMax = "100.0", UIMin = "1.0", UIMax = "2.0"))
    float StamineRegenIdleMultiplier = 1.8f;

  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Abilities|Granted abilities")
    TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

    UPROPERTY(EditAnywhere, Category = "SK Abilities|Granted abilities")
    TObjectPtr<USKAbilitiesDataAsset> GrantedAbilitiesDataAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Abilities|Default Abilities")
    TSubclassOf<UGameplayAbility> SprintingAbility;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Abilities|Default Abilities")
    TSubclassOf<UGameplayAbility> InteractAbility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Attributes|Regen Gameplay Effects")
    TObjectPtr<USKBasicGameplayEffectsDataAsset> BasicGameplayEffectsDataAsset = nullptr;

  private:
    void InitAbilities();
    void ValidateGEDataAssets() const;
    const float CalculateFallDamage(const float &FallSpeed);

    FGameplayEffectSpecHandle MakeGESpecHandle(const TSubclassOf<UGameplayEffect> &GameplayEffectClass);

    FActiveGameplayEffectHandle StaminaRegenActiveGESpecHandle;
    FActiveGameplayEffectHandle HealthRegenActiveGESpecHandle;

    UPROPERTY()
    FVector2D LandingSpeedRange = {700.0f, 1200.0f};
    UPROPERTY()
    FVector2D LandingDamageRange = {15.0f, 100.0f};
};
