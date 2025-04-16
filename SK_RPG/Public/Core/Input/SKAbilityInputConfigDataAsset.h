// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "SKAbilityInputConfigDataAsset.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FSKInputAction
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    const UInputAction *InputAction = nullptr;

    UPROPERTY(EditDefaultsOnly)
    FGameplayTag InputTag = FGameplayTag();
};

UCLASS()
class SIRKNIGHT_API USKAbilityInputConfigDataAsset : public UDataAsset
{
    GENERATED_BODY()

  public:
    const UInputAction *FindAbilityInputActionForTag(const FGameplayTag &InputTag, bool bLogNotFound = false) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FSKInputAction> AbilityInputActions;
};
