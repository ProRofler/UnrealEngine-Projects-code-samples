// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Characters/SKBaseCharacter.h"
#include "Core/EnhancedInputData.h"
#include "CoreMinimal.h"
#include "SKPlayerCharacter.generated.h"

class UCameraComponent;
struct FEnhancedInputData;

UCLASS()
class SIRKNIGHT_API ASKPlayerCharacter : public ASKBaseCharacter
{
    GENERATED_BODY()

  public:
    ASKPlayerCharacter(const FObjectInitializer &ObjectInitializer);
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Actor components")
    TObjectPtr<UPhysicsHandleComponent> PhysicsHandle;

  protected:
    // Player specific
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "_Player camera settings")
    TObjectPtr<UCameraComponent> PlayerCamera;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "_Enhanced input settings")
    FEnhancedInputData InputData;

    void HandleAlternativeAction();

    // interactions
    virtual void HandleInteractionActor() override;
    virtual void Interact() override;
    void UpdateGrabLocation();
    void RotateGrabbedComponent(const FVector2D& Input) const;
    float CheckDistanceToPlayer(const TObjectPtr<AActor> OtherComponent);

    TObjectPtr<UMeshComponent> GrabbedComponent;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interactions settings")
    float GrabDistance = 150.0f;

  private:
    // input related
    void ControllerSetup();
    UFUNCTION()
    void MoveAction(const FInputActionValue &Value);
    UFUNCTION()
    void LookingAction(const FInputActionValue &Value);

    // interactions
    void GetLookedAtActor(TObjectPtr<AActor> &LookedAtActor) const;
    FHitResult TraceToActor(const TObjectPtr<AActor> &OtherActor) const;
    bool TraceFromCamera(FHitResult &HitResult, const float TraceDistance);
    bool TraceFromCamera(FHitResult &HitResult, const float TraceDistance, const TObjectPtr<UMeshComponent> ComponentToIgnore);

    void HandleGrabbing();
    bool CanGrabItem();
    void GrabItem();
    void ReleaseItem();

    // debug
    void PrintDebugInfo();
};
