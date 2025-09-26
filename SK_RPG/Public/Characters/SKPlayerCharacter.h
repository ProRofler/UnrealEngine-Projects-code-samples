// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Characters/SKBaseCharacter.h"
#include "CoreMinimal.h"

#include "SKPlayerCharacter.generated.h"

class ASKPlayerController;
class ASKPlayerHUD;
class USKInventoryObjectData;

class USKListViewWidget;

class UCameraComponent;
class USKPhysicsHandleComponent;
class USKInteractionComponent;

UCLASS()
class SIRKNIGHT_API ASKPlayerCharacter : public ASKBaseCharacter
{
    GENERATED_BODY()

  public:
    friend USKPhysicsHandleComponent;
    friend ASKPlayerController;
    friend USKListViewWidget;
    friend USKInteractionComponent;

    ASKPlayerCharacter(const FObjectInitializer &ObjectInitializer);

    /************************************ UE INHERITED ******************************************/

  protected:
    virtual void BeginPlay() override;

    /************************************ UTILS ******************************************/

  public:
    UFUNCTION(BlueprintPure, Category = "SK Player | Utils")
    bool TraceFromCamera(FHitResult &HitResult, const float TraceDistance,
                         const UPrimitiveComponent *ComponentToIgnore = nullptr);


    /************************************ Player specific ******************************************/
  public:
    USKInteractionComponent *GetInteractionComponent_Implementation() const { return InteractionComponent.Get(); }

    UFUNCTION(BlueprintPure, Category = "SK Player | Getters")
    FORCEINLINE USkeletalMeshComponent *GetBodyMesh() { return Body; }

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SK Player Settings | Camera")
    TObjectPtr<UCameraComponent> PlayerCamera;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SK Actor components")
    TObjectPtr<USKPhysicsHandleComponent> PhysicsHandle;
    UPROPERTY()
    TObjectPtr<USKInteractionComponent> InteractionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SK Player Settings | Mesh")
    TObjectPtr<USkeletalMeshComponent> Body;

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

    void HandleAlternativeAction();
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
};
