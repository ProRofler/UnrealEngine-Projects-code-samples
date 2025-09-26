// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "S_CharacterMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class CPP_02_API US_CharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player movement",
              meta = (ClampMin = "1.0", ClampMax = "10.0"))
    float SprintModifier = 2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player movement",
              meta = (ClampMin = "1.0", ClampMax = "5000.0"))
    float Speed = 400.0f;

    virtual float GetMaxSpeed() const override;
};
