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

    void GrabItem(UPrimitiveComponent *ComponentToGrab);
    void ReleaseItem();
    void RotateGrabbedComponent(const FVector2D &Input);

  protected:
    virtual void BeginPlay() override;

  private:
    void UpdateGrabLocation();
    ASKPlayerCharacter *Player = nullptr;
    FRotator InitialRelativeRotation;
    float CheckDistanceToPlayer(const AActor *OtherActor);
};
