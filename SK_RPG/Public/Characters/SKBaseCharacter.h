// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interface/SKInterfaceCharacter.h"
#include "Core/SKCoreTypes.h"
#include "GameFramework/Character.h"
#include "SKBaseCharacter.generated.h"

class USKCharacterMovementComponent;
class USKInventoryComponent;
class UCapsuleComponent;
class ASKInteractableBase;
class UPhysicsHandleComponent;

UCLASS()
class SIRKNIGHT_API ASKBaseCharacter : public ACharacter, public ISKInterfaceCharacter
{
    GENERATED_BODY()

  public:
    ASKBaseCharacter(const FObjectInitializer &ObjectInitializer);

    virtual void Tick(float DeltaTime) override;

    // Getters
    UFUNCTION(BlueprintCallable)
    bool GetWantsTosprint() const { return bWantsToSprint; }
    UFUNCTION(BlueprintCallable)
    bool GetWalkToggle() const { return bWalkToggle; }
    EActionType GetActionType() const { return ActionType; }
    EMovementType GetMovementType() const { return MovementType; }
    const TWeakObjectPtr<AActor> &GetInteractibleActive() const { return InteractibleActive; }
    const TObjectPtr<USKInventoryComponent> &GetInventoryComponent() { return Inventory; }

    // setters
    void SetActionType(const EActionType _ActionType) { ActionType = _ActionType; }

    // movement related
    void StartSprinting();
    void StartWalking();
    void StartRunning();

  protected:
    virtual void BeginPlay() override;

    // interactions
    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UCapsuleComponent> InteractionZone;
    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<USKInventoryComponent> Inventory;

    TSet<AActor *> InteractablesInVicinity;
    TWeakObjectPtr<AActor> InteractibleActive;
    FTimerHandle InteractionTimer;

    void HandleInteractionsTimer();
    void AsyncInteractionHandle();
    virtual void HandleInteractionActor();
    virtual void TakeItem();

    mutable FRWLock DataGuard;

  private:
    bool bWantsToSprint = false;
    bool bWalkToggle = false;

    TObjectPtr<USKCharacterMovementComponent> MovementComponent;
    EMovementType MovementType = EMovementType::ERunning;
    EActionType ActionType = EActionType::ENone;

    // Interactions
    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                      int32 OtherBodyIndex);
};
