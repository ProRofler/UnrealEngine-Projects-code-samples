// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "S_PLayerCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class US_Health_Component;
class UTextRenderComponent;
class US_PlayerWeaponComponent;

UCLASS()
class CPP_02_API AS_PLayerCharacter : public ACharacter
{
    GENERATED_BODY()

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    void ControllerSetup();

    UFUNCTION()
    void MoveAction(const FInputActionValue &Value);

    UFUNCTION()
    void WantsToRun(const FInputActionValue &Value);

    UFUNCTION()
    void LookingAction(const FInputActionValue &Value);

    UFUNCTION()
    void OnGroundLanded(const FHitResult &Hit);

    UFUNCTION()
    virtual void OnDeath();

    // enhanced input
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputMappingContext *InputMapping = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputAction *MovingAction = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputAction *LookAction = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputAction *JumpAction = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputAction *SprintAction = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputAction *FireAction = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputAction *WeaponSwitchAction = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputAction *ReloadAction = nullptr;

    UPROPERTY()
    FVector2D LandingSpeedRange = {800.0f, 1200.0f};

    UPROPERTY()
    FVector2D LandingDamageRange = {15.0f, 100.0f};

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

    // Sets default values for this character's properties
    AS_PLayerCharacter(const FObjectInitializer &ObjInit);

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player components")
    UCameraComponent *PlayerCamera;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player components")
    USpringArmComponent *PlayerSpringarm = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PLayer health")
    US_Health_Component *PlayerHealthComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player health")
    UTextRenderComponent *HealthBarComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player weapon")
    US_PlayerWeaponComponent *PlayerWeaponComponent = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Player movement")
    bool bWantsToRun = false;

    UPROPERTY(EditDefaultsOnly)
    UAnimMontage *DeathAnimation = nullptr;

    UFUNCTION(BlueprintCallable, Category = "Player movement")
    float GetPlayerMovementAngle() const;

    // for input
    bool IsInverted = false;

    void OnHealthChanged(float health, float HealthDelta);
};
