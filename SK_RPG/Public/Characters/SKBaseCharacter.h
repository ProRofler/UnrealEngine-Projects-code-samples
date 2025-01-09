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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartedSprinting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartedRunning);

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

    /************************************ MOVEMENT  ******************************************/
  public:
    UFUNCTION(BlueprintCallable)
    void TrySrinting() const;
    UFUNCTION(BlueprintCallable)
    void TryRunning() const;
    UFUNCTION(BlueprintCallable)
    void TryWalking();
    UFUNCTION(BlueprintPure, Category = "Character movement")
    float GetCharacterMovementAngle();

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStartedSprinting OnStartedSprinting;
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStartedRunning OnStartedRunning;

    /************************************ State  ******************************************/
  protected:
    void HandleIdling();

    bool bIsReceivingInput = false;

    /************************************ Interactions  ******************************************/
  public:
    UFUNCTION(BlueprintCallable)
    const AActor *GetInteractibleActive() const { return InteractibleActive.Get(); }

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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USKAbilitySystemComponent> AbilitySystemComponent;
    TObjectPtr<const UAttributeSet> AttributeSet;
    TObjectPtr<const UAttributeSet> AttributeSetSkills;

    /************************************ GAS  ******************************************/
  public:
    void ActivateSprintAbility();

    /************************************ DEBUGGING  ******************************************/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DEBUGGING")
    bool bEnableLogging = true;
};
