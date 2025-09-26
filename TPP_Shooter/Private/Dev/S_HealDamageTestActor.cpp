// Fill out your copyright notice in the Description page of Project Settings.


#include "Dev/S_HealDamageTestActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "S_Health_Component.h"
#include "Dev/S_TestDamageType.h"

// Sets default values
AS_HealDamageTestActor::AS_HealDamageTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene component");
    SetRootComponent(SceneComponent);

}

// Called when the game starts or when spawned
void AS_HealDamageTestActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsDamaging)
        SphereColor = FColor::Red;
}

// Called every frame
void AS_HealDamageTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 24, SphereColor);

	if (IsDamaging)
	{
        UGameplayStatics::ApplyRadialDamage(GetWorld(), AffectAmount, GetActorLocation(), SphereRadius, DamageType, {},
                                            this, nullptr, FullDamage);	
	}


}

