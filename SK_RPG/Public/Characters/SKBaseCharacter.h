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
class USKAttributeSet;
class USKAttributeSetSkills;
class USKAbilitySystemComponent;
class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartedSprintingSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartedRunningSignature);
DECLARE_DELEGATE_OneParam(FOnStaminaChangedSignature, float);
DECLARE_DELEGATE_OneParam(FOnHealthChangedSignature, float);

UCLASS(meta = (PrioritizeCategories = "SK DEBUGGING"))
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
    /************************************ Attributes ******************************************/
  public:
    UFUNCTION(BlueprintPure)
    FORCEINLINE float GetStaminaPercent() const;
    UFUNCTION(BlueprintPure)
    FORCEINLINE float GetHealthPercent() const;

  private:
    void HandleStaminaChange(const float ChangedAmount);
    void HandleHealthChange(const float ChangedAmount);
    void HandleStaminaDepleted();

    bool IsStaminaFull() const;
    /************************************ MOVEMENT  ******************************************/
  public:
    UFUNCTION(BlueprintCallable)
    void StartSprinting() const;
    UFUNCTION(BlueprintCallable)
    void TryRunning() const;
    UFUNCTION(BlueprintCallable)
    void TryWalking();
    UFUNCTION(BlueprintCallable)
    void TryJumping();
    UFUNCTION(BlueprintCallable)
    void TrySprinting();
    UFUNCTION(BlueprintPure, Category = "SK Character movement")
    float GetCharacterMovementAngle() const;

    UFUNCTION(BlueprintPure, Category = "SK Character movement")
    bool IsMovingForward() const;

    UPROPERTY(BlueprintAssignable, Category = "SK Events")
    FOnStartedSprintingSignature OnStartedSprinting;
    UPROPERTY(BlueprintAssignable, Category = "SK Events")
    FOnStartedRunningSignature OnStartedRunning;

    FOnStaminaChangedSignature OnStaminaChanged;
    FOnHealthChangedSignature OnHealthChanged;

    /************************************ ACTIONS  ******************************************/

    UFUNCTION(BlueprintCallable) void TryDrawWeapon();

    /************************************ State  ******************************************/
  public:
    bool IsCharacterMoving() const;

  protected:
    virtual void HandleIdling();
    void StartIdle();
    void StopIdle();

  private:
    void HandleIdleTimer();

    /************************************ Interactions  ******************************************/
  public:
    UFUNCTION(BlueprintCallable, Category = "SK Interactions")
    const AActor *GetInteractionTarget() const { return InteractionTarget.Get(); }

  protected:
    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UCapsuleComponent> InteractionZone;
    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<USKInventoryComponent> Inventory;

    TSet<AActor *> InteractablesInVicinity;
    TWeakObjectPtr<AActor> InteractionTarget;

    void HandleInteractionsTimer();
    virtual void HandleInteractionActor();
    virtual void Interact();

    /************************************ MULTITHREADING  ******************************************/
  protected:
    mutable FRWLock DataGuard;

    void AsyncInteractionHandle();

    /************************************ COMPONENTS  ******************************************/
  public:
    const TObjectPtr<USKInventoryComponent> &GetInventoryComponent() { return Inventory; }
    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

  protected:
    TObjectPtr<USKCharacterMovementComponent> MovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SK GAS", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USKAbilitySystemComponent> AbilitySystemComponent;
    TObjectPtr<const USKAttributeSet> AttributeSet;
    TObjectPtr<const USKAttributeSetSkills> AttributeSetSkills;

    /************************************ Timers ******************************************/
  private:
    FTimerHandle StaminaRegenTimerHandle;
    FTimerHandle InteractionTimer;

    /************************************ DEBUGGING  ******************************************/
  public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Logging", meta = (DisplayPriority = 1))
    bool bEnableLogging = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Logging", meta = (EditCondition = "bEnableLogging"))
    bool bEnableLoggingAbilitySystem = true;
};
