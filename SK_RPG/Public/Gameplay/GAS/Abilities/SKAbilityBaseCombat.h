// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/Abilities/SKAbilityBase.h"
#include "GameplayEffect.h"

#include "SKAbilityBaseCombat.generated.h"

UCLASS()
class SIRKNIGHT_API USKAbilityBaseCombat : public USKAbilityBase
{
    GENERATED_BODY()

  public:
    // Damage effect is optional
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SK Damage",
              meta = (ToolTip = "Optional Gameplay Effect. If not set, it will be ignored."))
    TSubclassOf<UGameplayEffect> DamageEffect;

  protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData *TriggerEventData);

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                            bool bWasCancelled);

  private:
    void SetupWeaponTrace();

    UFUNCTION()
    void OnGameplayEventTrace(FGameplayEventData Payload);
    UFUNCTION()
    void OnGameplayEventHit(FGameplayEventData Payload);
};
