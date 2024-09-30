// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/S_PickupBase.h"
#include "Components/SphereComponent.h"
#include "S_Health_Component.h"

// Sets default values
AS_PickupBase::AS_PickupBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision component");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);
}


// Called when the game starts or when spawned
void AS_PickupBase::BeginPlay()
{
    Super::BeginPlay();
    GenerateRotationAngle();
}

void AS_PickupBase::NotifyActorBeginOverlap(AActor *OtherActor)
{
    const auto HealthComponent = OtherActor->GetComponentByClass<US_Health_Component>();
    if (!HealthComponent || HealthComponent->IsDead())
        return;

    PickUp();
    OnPickup(OtherActor);

    // Destroy();
}

// Called every frame
void AS_PickupBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator(0.0f, rotationAngle, 0.0f));
}

void AS_PickupBase::OnPickup(AActor *PlayerActor)
{
    UE_LOG(LogTemp, Error,
           TEXT("Picked up base pickup class, this should not ever happen. Override the pickup function"));
    checkNoEntry();
}

void AS_PickupBase::PickUp()
{

    GetRootComponent()->SetVisibility(false, true);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    
    GetWorldTimerManager().SetTimer(Timer, this, &AS_PickupBase::PickupRespawn, RespawnRate);
}

void AS_PickupBase::PickupRespawn()
{
    GenerateRotationAngle();
    GetRootComponent()->SetVisibility(true, true);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void AS_PickupBase::GenerateRotationAngle()
{
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;
    rotationAngle = FMath::RandRange(1.0f, 2.0f) * Direction;
}

bool AS_PickupBase::CouldBeTaken() const 
{
    return !GetWorldTimerManager().IsTimerActive(Timer);
}