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

    UFUNCTION(BlueprintCallable, Category = "SK Character|Attributes")
    bool IsStaminaFull() const;
    UFUNCTION(BlueprintCallable, Category = "SK Character|Attributes")
    bool IsHeathFull() const;
    UFUNCTION(BlueprintCallable, Category = "SK Character|Attributes")
    bool IsDead() const;

    UFUNCTION()
    virtual void HandleDeath();

  private:
    void HandleStaminaChange(const float ChangedAmount);
    void HandleStaminaDepleted();
    void HandleHealthChange(const float ChangedAmount);

    template <typename T>
    FORCEINLINE void HandleRegenTimer(FTimerHandle &TimerHandle, T *Object, void (T::*Function)(), float LoopingTime,
                                      float InitialDelay)
    {
        if (GetWorldTimerManager().IsTimerActive(TimerHandle))
        {
            GetWorldTimerManager().ClearTimer(TimerHandle);
            GetWorldTimerManager().SetTimer(TimerHandle, Object, Function, LoopingTime, false, InitialDelay);
        }
        else
        {
            GetWorldTimerManager().SetTimer(TimerHandle, Object, Function, LoopingTime, false, InitialDelay);
        }
    }
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
    UFUNCTION(BlueprintPure, Category = "SK Character|movement")
    float GetCharacterMovementAngle() const;

    UFUNCTION(BlueprintPure, Category = "SK Character|movement")
    bool IsMovingForward() const;

    virtual void Landed(const FHitResult &Hit);

    UPROPERTY(BlueprintAssignable, Category = "SK Character|Events")
    FOnStartedSprintingSignature OnStartedSprinting;
    UPROPERTY(BlueprintAssignable, Category = "SK Character|Events")
    FOnStartedRunningSignature OnStartedRunning;

    FOnStaminaChangedSignature OnStaminaChanged;
    FOnHealthChangedSignature OnHealthChanged;

    /************************************ ACTIONS  ******************************************/

    UFUNCTION(BlueprintCallable) void TryDrawWeapon();

    /************************************ State  ******************************************/
  public:
    bool IsCharacterMoving() const;
    virtual bool CanJumpInternal_Implementation() const override;

    UFUNCTION(BlueprintPure, Category = "SK Character|State")
    bool CanSprint(uint8 RequiredStaminaPercentage = 25) const;

  protected:
    virtual void HandleIdling();
    void StartIdle();
    void StopIdle();

    /************************************ Interactions  ******************************************/
  public:
    UFUNCTION(BlueprintCallable, Category = "SK Character|Interactions")
    const AActor *GetInteractionTarget() const { return InteractionTarget.Get(); }

  protected:
    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UCapsuleComponent> InteractionZone;

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
    USKInventoryComponent *GetInventoryComponent_Implementation() const { return InventoryComponent; };
    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

  protected:
    TObjectPtr<USKCharacterMovementComponent> MovementComponent;
    UPROPERTY(VisibleAnywhere, Category = "SK Character|Inventory")
    TObjectPtr<USKInventoryComponent> InventoryComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SK Character|GAS", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USKAbilitySystemComponent> AbilitySystemComponent;
    TObjectPtr<const USKAttributeSet> AttributeSet;
    TObjectPtr<const USKAttributeSetSkills> AttributeSetSkills;

    /************************************ Timers ******************************************/
  private:
    FTimerHandle StaminaRegenTimerHandle;
    FTimerHandle HealthRegenTimerHandle;
    FTimerHandle InteractionTimer;

    /************************************ DEBUGGING  ******************************************/
  public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Logging", meta = (DisplayPriority = 1))
    bool bEnableLogging = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Logging", meta = (EditCondition = "bEnableLogging"))
    bool bEnableLoggingAbilitySystem = true;
};
