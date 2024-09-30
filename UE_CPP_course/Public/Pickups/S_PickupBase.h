// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S_PickupBase.generated.h"


class USphereComponent;

UCLASS()
class CPP_02_API AS_PickupBase : public AActor
{
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    AS_PickupBase();

    bool CouldBeTaken() const;

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup settings")
    float RespawnRate = 5.0f;

    UPROPERTY()
    float rotationAngle = 0.0f;

    UPROPERTY()
    USphereComponent * CollisionComponent;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    virtual void OnPickup(AActor* PlayerActor);
    void PickUp();
    void PickupRespawn();
    void GenerateRotationAngle();

    FTimerHandle Timer;
};
