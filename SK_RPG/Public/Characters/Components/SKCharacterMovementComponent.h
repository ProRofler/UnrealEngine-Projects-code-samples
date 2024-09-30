// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/SKCoreTypes.h"
#include "SKCharacterMovementComponent.generated.h"

UCLASS()
class SIRKNIGHT_API USKCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

  public:
    USKCharacterMovementComponent(const FObjectInitializer &ObjectInitializer);
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "__TEMP Skills")
    float Athletics = 35.0f;
    
    void SetCharacterSpeed(const EMovementType& MovementType);

    /*void StartWalking();
    void StartSprinting();*/

  protected:
    

  private:
    float BaseWalkSpeed;
    AActor *Character = nullptr;
};
