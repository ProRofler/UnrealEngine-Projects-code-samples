// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"

#include "Core/Interface/SKInterfaceCharacter.h"

#include "Core/SKCoreTypes.h"

#include "Utils/DataAssets/SKCharacterAnimationsDataAsset.h"

#include "Gameplay/GAS/SKAttributeSet.h"

#include "SKBaseCharacter.generated.h"

class USKCharacterMovementComponent;
class USKInventoryComponent;
class USKWeaponComponent;
class UPhysicsHandleComponent;
class UAbilitySystemComponent;

class USKAttributeSet;
class USKAttributeSetSkills;
class USKAbilitySystemComponent;

class USKInventoryObjectData;

class ASKInteractableBase;

class UAttributeSet;
class UGameplayAbility;

class USKAbilitiesDataAsset;
class USKBasicGameplayEffectsDataAsset;

DECLARE_DELEGATE_OneParam(FOnMainAttributeChangedSignature, FSKAttributeChangeData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeathSignature);

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

    /************************************ Attributes ******************************************/
  public:
    UFUNCTION(BlueprintPure)
    float GetStaminaPercent() const;
    UFUNCTION(BlueprintPure)
    float GetHealthPercent() const;

    UFUNCTION(BlueprintPure, Category = "SK Character|Attributes")
    bool IsStaminaFull() const;
    UFUNCTION(BlueprintPure, Category = "SK Character|Attributes")
    bool IsHeathFull() const;
    UFUNCTION(BlueprintPure, Category = "SK Character|Attributes")
    bool IsDead() const;

    UFUNCTION(BlueprintCallable, Category = "SK Character|Attributes")
    virtual void HandleDeath();

    UPROPERTY(BlueprintAssignable)
    FOnCharacterDeathSignature OnCharacterDeath;

  private:
    void HandleMainAttributeChange(const FSKAttributeChangeData ChangedAttributeInfo);

    /************************************ MOVEMENT  ******************************************/
  public:
    UFUNCTION(BlueprintCallable, Category = "SK Character movement")
    void TrySprinting() const;
    UFUNCTION(BlueprintCallable, Category = "SK Character movement")
    void TryRunning() const;
    UFUNCTION(BlueprintCallable, Category = "SK Character movement")
    void TryWalking();

    UFUNCTION(BlueprintPure, Category = "SK Character|movement")
    float GetCharacterMovementAngle() const;

    UFUNCTION(BlueprintPure, Category = "SK Character|movement")
    bool IsMovingForward() const;

    virtual void Landed(const FHitResult &Hit);

    FOnMainAttributeChangedSignature OnMainAttributeChanged;

  protected:
    UFUNCTION(BlueprintCallable, Category = "SK Character|Physics")
    void ActivateRagdoll();

    UFUNCTION(BlueprintCallable, Category = "SK Character|Physics")
    void DeactivateRagdoll();

    UFUNCTION(BlueprintPure, Category = "SK Character|Physics")
    bool IsMeshFacingUpwards() const;

  private:
    void UpdateCapsuleTransform(const float Delta);

    bool bShouldUpdateCapsuleTransform = false;

    /************************************ ACTIONS  ******************************************/
  public:
    void HandleUseItem(USKInventoryObjectData *ObjectData);

    UFUNCTION()
    void OnCapsuleBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    /************************************ State  ******************************************/
  public:
    bool IsCharacterMoving() const;

    UFUNCTION(BlueprintPure, Category = "SK Character|State")
    bool CanSprint(uint8 RequiredStaminaPercentage = 25) const;

  protected:
    virtual void HandleIdling();
    void StartIdle();
    void StopIdle();

    /************************************ COMPONENTS  ******************************************/
  public:
    USKInventoryComponent *GetInventoryComponent_Implementation() const { return InventoryComponent.Get(); }
    USKWeaponComponent *GetWeaponComponent_Implementation() const { return WeaponComponent.Get(); }
    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Character|Inventory")
    TObjectPtr<UDataTable> StartingInventoryData;

  protected:
    UPROPERTY()
    TObjectPtr<USKCharacterMovementComponent> MovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SK Character|Inventory")
    TObjectPtr<USKInventoryComponent> InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SK Character|Weapon")
    TObjectPtr<USKWeaponComponent> WeaponComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SK Character|GAS", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USKAbilitySystemComponent> AbilitySystemComponent;
    UPROPERTY()
    TObjectPtr<const USKAttributeSet> AttributeSet;
    UPROPERTY()
    TObjectPtr<const USKAttributeSetSkills> AttributeSetSkills;

    /************************************ Data assets  ******************************************/
  public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Character|Data Assets")
    USKCharacterAnimationsDataAsset *AnimationsDA = nullptr;

    UPROPERTY(EditAnywhere, Category = "SK Ability System|Data Assets")
    TObjectPtr<USKAbilitiesDataAsset> GrantedAbilitiesDataAsset;

    UPROPERTY(EditAnywhere, Category = "SK Ability System|Data Assets")
    TObjectPtr<USKBasicGameplayEffectsDataAsset> BasicGameplayEffects;

    /************************************ DEBUGGING  ******************************************/
  public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Logging", meta = (DisplayPriority = 1))
    bool bEnableLogging = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Logging", meta = (EditCondition = "bEnableLogging"))
    bool bEnableLoggingAbilitySystem = true;
};
