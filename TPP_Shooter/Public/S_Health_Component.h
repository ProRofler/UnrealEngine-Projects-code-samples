// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "S_Health_Component.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, float)

DECLARE_LOG_CATEGORY_EXTERN(LogCharacterDamage, All, All)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

//class UCameraShakeBase;

class CPP_02_API US_Health_Component : public UActorComponent
{
    GENERATED_BODY()

  public:
    // Sets default values for this component's properties
    US_Health_Component();

    // Vars
    UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = "1.0", ClampMax = "100.0"))
    float startHealth = 100;

    UPROPERTY(EditAnywhere, Category = "Health")
    bool GodMode = false;

    UPROPERTY(EditDefaultsOnly, Category = "Auto Heal")
    bool AutoHeal = true;
    UPROPERTY(EditDefaultsOnly, Category = "Auto Heal", meta = (Condition = "AutoHeal"))
    float HealRate = 3.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Auto Heal", meta = (Condition = "AutoHeal"))
    float HealAmount = 0.1f;

    UPROPERTY(EditDefaultsOnly, Category = "Auto Heal", meta = (Condition = "AutoHeal"))
    float HealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Auto Heal", meta = (Condition = "AutoHeal"))
    TSubclassOf<UCameraShakeBase> CameraShake;

    // Functions
    float GetHealth() const
    {
        return health;
    }

    UFUNCTION(BlueprintCallable)
    bool IsDead() const;

    UFUNCTION(BlueprintCallable)
    void AddHealth(float HealthAmount);

    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const {return health/startHealth;};

    FOnDeath OnDeath;
    FOnHealthChange OnHealthChange;

  protected:
    // Called when the game starts
    virtual void BeginPlay() override;

  private:
    float health = FMath::Clamp(health, 0.0f, startHealth);

    FTimerHandle AutoHealTimerHandle;

    UFUNCTION()
    void HandleDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
                      class AController *InstigatedBy, AActor *DamageCauser);

    UFUNCTION()
    void StartAutoHeal();

    UFUNCTION()
    void SetHealth(float HealthAmount);
    void ShakeCamera();

};
