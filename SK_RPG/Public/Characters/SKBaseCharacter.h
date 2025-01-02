// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "Core/Interface/SKInterfaceCharacter.h"
#include "Core/SKCoreTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SKBaseCharacter.generated.h"

class USKCharacterMovementComponent;
class USKInventoryComponent;
class UCapsuleComponent;
class ASKInteractableBase;
class UPhysicsHandleComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class USKAbilitySystemComponent;
class UGameplayAbility;

UCLASS()
class SIRKNIGHT_API ASKBaseCharacter : public ACharacter, public ISKInterfaceCharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

  public:
    ASKBaseCharacter(const FObjectInitializer &ObjectInitializer);

    /************************************ UE INHERITED ******************************************/

  public:
    virtual void Tick(float DeltaTime) override;

  protected:
    virtual void BeginPlay() override;

  private:
    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                      int32 OtherBodyIndex);

    /************************************ GETTERS ******************************************/
  public:
    UFUNCTION(BlueprintCallable)
    bool GetWantsTosprint() const { return bWantsToSprint; }
    UFUNCTION(BlueprintCallable)
    bool GetWalkToggle() const { return bWalkToggle; }
    EActionType GetActionType() const { return ActionType; }
    EMovementType GetMovementType() const { return MovementType; }
    const TWeakObjectPtr<AActor> &GetInteractibleActive() const { return InteractibleActive; }
    const TObjectPtr<USKInventoryComponent> &GetInventoryComponent() { return Inventory; }
    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    /************************************ SETTERS ******************************************/
    void SetActionType(const EActionType _ActionType) { ActionType = _ActionType; }

    /************************************ MOVEMENT  ******************************************/
  public:
    UFUNCTION(BlueprintCallable)
    void StartSrinting() const;
    UFUNCTION(BlueprintCallable)
    void StartRunning() const;

    // old
    void StartWalking();
    void StartRunning();

    /************************************ Interactions  ******************************************/
  protected:
    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UCapsuleComponent> InteractionZone;
    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<USKInventoryComponent> Inventory;

    TSet<AActor *> InteractablesInVicinity;
    TWeakObjectPtr<AActor> InteractibleActive;
    FTimerHandle InteractionTimer;

    void HandleInteractionsTimer();
    virtual void HandleInteractionActor();
    virtual void TakeItem();

    /************************************ MULTITHREADING  ******************************************/
  protected:
    mutable FRWLock DataGuard;

    void AsyncInteractionHandle();

    /************************************ COMPONENTS  ******************************************/
  protected:
    TObjectPtr<USKCharacterMovementComponent> MovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USKAbilitySystemComponent> AbilitySystemComponent;
    TObjectPtr<const UAttributeSet> AttributeSet;
    TObjectPtr<const UAttributeSet> AttributeSetSkills;

    /************************************ GAS  ******************************************/
  public:
    void ActivateSprintAbility();

    /************************************ MISC\DEPRECATED  ******************************************/
  public:
    EMovementType MovementType = EMovementType::ERunning;
    EActionType ActionType = EActionType::ENone;

  private:
    bool bWantsToSprint = false;
    bool bWalkToggle = false;
};
