// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "SKPhysicsHandleComponent.generated.h"

class ASKPlayerCharacter;

UCLASS()
class SIRKNIGHT_API USKPhysicsHandleComponent : public UPhysicsHandleComponent
{
    GENERATED_BODY()

  public:
    USKPhysicsHandleComponent();

    // getters
    const TObjectPtr<UMeshComponent> &GetItemToGrab() const { return ItemToGrab; }

    void GrabItem();
    void ReleaseItem();
    void RotateGrabbedComponent(const FVector2D &Input);

  private:
    void UpdateGrabLocation();
    ASKPlayerCharacter *Player = nullptr;
    TObjectPtr<UMeshComponent> ItemToGrab;
    FRotator InitialRelativeRotation;
    float CheckDistanceToPlayer(const AActor *OtherActor);
};
