// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SKAbilitiesDataAsset.generated.h"

UCLASS()
class SIRKNIGHT_API USKAbilitiesDataAsset : public UDataAsset
{
    GENERATED_BODY()

  public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Abilities")
    TArray<TSubclassOf<class UGameplayAbility>> ActiveAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Abilities")
    TArray<TSubclassOf<class UGameplayAbility>> PassiveAbilities;
};
