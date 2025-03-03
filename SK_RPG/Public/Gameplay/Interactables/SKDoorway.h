// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"

#include "Core/Interface/SKInterfaceLockable.h"

#include "Gameplay/Interactables/SKInteractableBase.h"

#include "SKDoorway.generated.h"

class UCurveFloat;
class USKLockComponent;

UCLASS()
class SIRKNIGHT_API ASKDoorway : public ASKInteractableBase, public ISKInterfaceLockable
{
    GENERATED_BODY()

  public:
    ASKDoorway();

    virtual void BeginPlay();
    virtual void Tick(float DeltaSeconds);

    virtual void OnConstruction(const FTransform &Transform);

    virtual void OnInteraction_Implementation(const AActor *TriggeredActor) override;

    UPROPERTY(EditAnywhere, Category = "SK Object settings")
    bool bShowGhostDoor = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SK Object settings")
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(VisibleAnywhere, Category = "SK Object settings")
    TObjectPtr<USceneComponent> OpenedDoorHandleComponent;

    UPROPERTY()
    TObjectPtr<UStaticMeshComponent> DoorGhostMesh;

    FORCEINLINE bool GetIsActive() const { return bIsActive; }

    /********************LOCKABLE INTERFACE*******************************/
  public:
    const USKLockComponent *GetLockComponent_Implementation() const { return LockComponent; }

    /*********************************************************************/

  protected:
    UPROPERTY(EditAnywhere, Category = "SK Doorway properties")
    bool bRemoteActivationOnly = false;

    UPROPERTY(EditAnywhere, Category = "SK Doorway properties", meta = (EditCondition = "!bRemoteActivationOnly"))
    bool bHasLock = false;

    UPROPERTY(EditAnywhere, Category = "SK Doorway properties")
    UCurveFloat *AnimationCurve;

    UFUNCTION()
    void HandleDoorAnimation(float Value);

    UPROPERTY(VisibleAnywhere, meta = (EditCondition = "bHasLock"))
    TObjectPtr<USKLockComponent> LockComponent = nullptr;

  private:
    FTimeline DoorTimeline;

    bool bIsOpened = false;
    bool bIsActive = false;
    FTransform StartTransform, EndTransform;

    UPROPERTY()
    UMaterialInterface *GhostMat = nullptr;

    void SetupGhostMaterial();
    void AssignGhostMaterial();
    void HandleDoorOpenClose();
};
