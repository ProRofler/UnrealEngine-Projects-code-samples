// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Characters/SKBaseCharacter.h"
#include "Core/EnhancedInputData.h"
#include "CoreMinimal.h"
#include "SKPlayerCharacter.generated.h"

class UCameraComponent;
class ASKPlayerController;
class ASKPlayerHUD;
class USKInventoryWidget;
class USKPhysicsHandleComponent;

UCLASS()
class SIRKNIGHT_API ASKPlayerCharacter : public ASKBaseCharacter
{
    GENERATED_BODY()

  public:
    friend USKPhysicsHandleComponent;
    friend USKInventoryWidget;

    ASKPlayerCharacter(const FObjectInitializer &ObjectInitializer);
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
    virtual void Tick(float DeltaTime) override;

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
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Actor components")
    TObjectPtr<USKPhysicsHandleComponent> PhysicsHandle;

    void HandleAlternativeAction();

    // interactions
    virtual void HandleInteractionActor() override;
    virtual void Interact() override;
    void DropItem(AActor *ItemToDrop);

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interactions settings")
    float GrabDistance = 150.0f;

  private:
    // player components
    TWeakObjectPtr<ASKPlayerController> PlayerController;

    void InitializeComponents();

    // input related
    void ControllerSetup();
    UFUNCTION()
    void MoveAction(const FInputActionValue &Value);
    UFUNCTION()
    void LookingAction(const FInputActionValue &Value);

    // UI
    void HandleInventoryToggle();

    // interactions
    AActor *GetLookedAtActor() const;
    bool TraceFromCamera(FHitResult &HitResult, const float TraceDistance, const UMeshComponent *ComponentToIgnore);
    FHitResult TraceToActor(const AActor *OtherActor) const;

    // grabbing
    void HandleGrabbing();
    bool CanGrabItem();

    // debug
    void PrintDebugInfo();
};
