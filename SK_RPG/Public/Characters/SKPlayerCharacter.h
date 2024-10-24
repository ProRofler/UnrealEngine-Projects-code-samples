// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Characters/SKBaseCharacter.h"
#include "Core/EnhancedInputData.h"
#include "CoreMinimal.h"
#include "SKPlayerCharacter.generated.h"

struct FEnhancedInputData;

class UCameraComponent;
class APlayerController;
class ASKPlayerHUD;
class USKInventoryWidget;
class USKPhysicsHandleComponent;

UCLASS()
class SIRKNIGHT_API ASKPlayerCharacter : public ASKBaseCharacter
{
    GENERATED_BODY()

  public:
    friend USKPhysicsHandleComponent;
    ASKPlayerCharacter(const FObjectInitializer &ObjectInitializer);
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Actor components")
    TObjectPtr<USKPhysicsHandleComponent> PhysicsHandle;

    // ******** UTILS *****
    bool TraceFromCamera(FHitResult &HitResult, const float TraceDistance);

  protected:
    // super
    virtual void BeginPlay() override;

    // Player specific
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "_Player camera settings")
    TObjectPtr<UCameraComponent> PlayerCamera;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "_Enhanced input settings")
    FEnhancedInputData InputData;

    void HandleAlternativeAction();

    // interactions
    virtual void HandleInteractionActor() override;
    virtual void Interact() override;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interactions settings")
    float GrabDistance = 150.0f;

  private:
    // player components
    TWeakObjectPtr<APlayerController> PlayerController;
    TWeakObjectPtr<ASKPlayerHUD> PlayerHUD;
    TWeakObjectPtr<USKInventoryWidget> PlayerInventoryWidget;

    void InitializeComponents();

    // input related
    void ControllerSetup();
    UFUNCTION()
    void MoveAction(const FInputActionValue &Value);
    UFUNCTION()
    void LookingAction(const FInputActionValue &Value);

    // interactions
    void GetLookedAtActor(TObjectPtr<AActor> &LookedAtActor) const;
    bool TraceFromCamera(FHitResult &HitResult, const float TraceDistance,
                         const TObjectPtr<UMeshComponent> ComponentToIgnore);
    FHitResult TraceToActor(const TObjectPtr<AActor> &OtherActor) const;

    // grabbing
    void HandleGrabbing();
    bool CanGrabItem();

    // debug
    void PrintDebugInfo();
};
