// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "SKCharacterMovementComponent.generated.h"

class USKAttributeSetSkills;
class ASKBaseCharacter;
class USKCharacterComponentBase;

UCLASS()
class SIRKNIGHT_API USKCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

  public:
    USKCharacterMovementComponent(const FObjectInitializer &ObjectInitializer);
    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

    void StartRunning();
    void StartSprinting();
    void StartWalking();

    UFUNCTION(BlueprintPure, Category = "SK Character")
    FORCEINLINE ASKBaseCharacter *GetSKOwnerCharacter() const;

  private:
    void HandleRunningSpeed();

    // for saving the speed from CMC
    float BaseWalkSpeed;

    TObjectPtr<USKCharacterComponentBase> SKCharacterComponentBase;

    FCriticalSection CriticalSection;
};
