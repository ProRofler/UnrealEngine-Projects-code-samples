// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "SKCharacterAnimationsDataAsset.generated.h"

UCLASS()
class SIRKNIGHT_API USKCharacterAnimationsDataAsset : public UDataAsset
{
    GENERATED_BODY()

  public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon animations")
    UAnimMontage *SheatheUnsheatheAnimMontage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat animations")
    UAnimMontage *AttackComboAnimMontage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat animations")
    UAnimMontage *BlockingAnimMontage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat animations")
    UAnimMontage *HitReactionAnimMontage = nullptr;
};
