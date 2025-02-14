// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "Gameplay/Interactables/SKInteractableBase.h"

#include "SKDoorway.generated.h"

class UCurveFloat;

UCLASS()
class SIRKNIGHT_API ASKDoorway : public ASKInteractableBase
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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SK Object settings")
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(EditAnywhere, Category = "SK Object settings")
    TObjectPtr<USceneComponent> OpenedDoorHandleComponent;

    UPROPERTY()
    TObjectPtr<UStaticMeshComponent> DoorGhostMesh;

    FORCEINLINE bool GetIsActive() const { return bIsActive; }

  protected:
    UPROPERTY(EditAnywhere, Category = "SK Doorway properties")
    bool bRemoteActivationOnly = false;

    UPROPERTY(EditAnywhere, Category = "SK Doorway properties")
    UCurveFloat *AnimationCurve;

    UFUNCTION()
    void HandleDoorOpenClose(float Value);

  private:
    FTimeline DoorTimeline;

    bool bIsOpened = false;
    bool bIsActive = false;
    FTransform StartTransform, EndTransform;

    UPROPERTY()
    UMaterialInterface *GhostMat = nullptr;

    void SetupGhostMaterial();
    void AssignGhostMaterial();
};
