// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Characters/SKBaseCharacter.h"
#include "CoreMinimal.h"

#include "SKPlayerCharacter.generated.h"

class UCameraComponent;
class ASKPlayerController;
class ASKPlayerHUD;
class USKInventoryWidget;
class USKPhysicsHandleComponent;
class USKInventoryObjectData;

UCLASS()
class SIRKNIGHT_API ASKPlayerCharacter : public ASKBaseCharacter
{
    GENERATED_BODY()

  public:
    friend USKPhysicsHandleComponent;
    friend USKInventoryWidget;
    friend ASKPlayerController;

    ASKPlayerCharacter(const FObjectInitializer &ObjectInitializer);

    /************************************ UE INHERITED ******************************************/

  protected:
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    /************************************ UTILS ******************************************/

  public:
    bool TraceFromCamera(FHitResult &HitResult, const float TraceDistance);

  private:
    AActor *GetLookedAtActor() const;
    bool TraceFromCamera(FHitResult &HitResult, const float TraceDistance,
                         const UPrimitiveComponent *ComponentToIgnore);
    FHitResult TraceToActor(const AActor *OtherActor) const;
    FHitResult TraceToBoundingBox(const AActor *OtherActor) const;

    /************************************ Player specific ******************************************/
  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SK Player camera")
    TObjectPtr<UCameraComponent> PlayerCamera;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SK Actor components")
    TObjectPtr<USKPhysicsHandleComponent> PhysicsHandle;

    /************************************ Attributes ******************************************/
  private:
    virtual void HandleDeath() override;

    /************************************ Input ******************************************/
  private:
    UFUNCTION()
    void MoveAction(const FInputActionValue &Value);
    UFUNCTION()
    void LookingAction(const FInputActionValue &Value);

    /************************************ State  ******************************************/
  private:
    virtual void HandleIdling() override;

    bool bIsLookIdle = false;
    /************************************ Interactions ******************************************/
  protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SK Interactions settings")
    float GrabDistance = 150.0f;

    void HandleAlternativeAction();
    virtual void HandleInteractionActor() override;
    void DropItem(USKInventoryObjectData *ItemToRemove, const int32 QuantityToDrop);

    UFUNCTION(BlueprintCallable, Category = "SK Grabbing")
    void HandleGrabbing();
    UFUNCTION(BlueprintPure, Category = "SK Grabbing")
    bool CanGrabItem();

  private:
    TWeakObjectPtr<ASKPlayerController> SKPlayerController = nullptr;

    /************************************ DEBUG ******************************************/
  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SK Logging", meta = (EditCondition = "bEnableLogging"))
    bool bEnableLoggingInput = true;

  private:
    void PrintDebugInfo();
};
