// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "SKAbilitySystemComponent.generated.h"

UCLASS()
class SIRKNIGHT_API USKAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()
  public:
    friend class ASKBaseCharacter;

    virtual void InitializeComponent() override;

    UFUNCTION(BlueprintCallable)
    bool IsAbilityActive(TSubclassOf<UGameplayAbility> AbilityClass) const;
    UFUNCTION(BlueprintCallable)
    bool CancelAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass);

    UFUNCTION(BlueprintCallable)
    TArray<UGameplayAbility *> &GetGrantedAbilities() { return GrantedAbilities; };

  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Granted abilities")
    TArray<UGameplayAbility *> GrantedAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Default Abilities")
    TSubclassOf<UGameplayAbility> SprintingAbility;

  private:
    void InitAbilities();
};
