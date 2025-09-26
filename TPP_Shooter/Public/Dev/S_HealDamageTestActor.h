// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S_HealDamageTestActor.generated.h"

UCLASS()
class CPP_02_API AS_HealDamageTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS_HealDamageTestActor();

	USceneComponent *SceneComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere settings")
    float SphereRadius = 240.0f;

	UPROPERTY()
    FColor SphereColor = FColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere settings")
    bool IsDamaging = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere settings")
    bool FullDamage = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere settings")
	float AffectAmount = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere settings")
	TSubclassOf<UDamageType> DamageType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
