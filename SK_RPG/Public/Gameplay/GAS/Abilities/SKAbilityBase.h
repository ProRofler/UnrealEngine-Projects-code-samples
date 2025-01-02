// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "SKAbilityBase.generated.h"

class ASKBaseCharacter;

UCLASS()
class SIRKNIGHT_API USKAbilityBase : public UGameplayAbility
{
    GENERATED_BODY()

  public:
    USKAbilityBase();

    virtual void OnGiveAbility(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec);

    UFUNCTION(BlueprintCallable, Category = "Ability references to basics")
    ASKBaseCharacter *GetOwnerCharacter() const { return OwnerCharacter; }

  protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData *TriggerEventData);

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                            bool bWasCancelled);

  private:
    ASKBaseCharacter *OwnerCharacter = nullptr;
};
