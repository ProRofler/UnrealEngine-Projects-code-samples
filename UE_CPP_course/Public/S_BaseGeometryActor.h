// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "S_BaseGeometryActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnColorChanged, const FLinearColor&, Color, const FString&, Name);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimerFinished, AActor*);

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Sin,
	Static
};

USTRUCT(BlueprintType)
struct FGeometryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	EMovementType MoveType = EMovementType::Static;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float amplitude = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float frequency = 2.0f;


};

USTRUCT(BlueprintType)
struct FMaterialData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material settings")
	FLinearColor Color = FLinearColor::Black;

	UPROPERTY(EditAnywhere, Category = "Material settings")
	float Metallic = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Material settings")
	float TimerAmount = 1.0f;


	UPROPERTY(EditAnywhere, Category = "Material settings")
	int32 TimerStep = 5;

};


UCLASS()
class CPP_02_API AS_BaseGeometryActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS_BaseGeometryActor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	void SetGeo(const FGeometryData &NewGeoData)
	{
		GeometryData = NewGeoData;
	}

	void SetMaterial(const FMaterialData &NewMatData)
	{
		MaterialData = NewMatData;
	}


	UPROPERTY(BlueprintAssignable)
	FOnColorChanged OnColorChanged;

	FOnTimerFinished OnTimerFinished;


	UFUNCTION(BlueprintCallable)
	FGeometryData GetGeoData() const
	{
		return GeometryData;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the object is destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry data")
	FGeometryData GeometryData;

	UPROPERTY(EditAnywhere, Category = "Material settings")
	FMaterialData MaterialData;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 AmmoCount = 25;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	int32 DeathAmount = 4;

	UPROPERTY(EditInstanceOnly, Category = "Health")
	double Health = 78.255685f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool IsFalling = false;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	bool IsInjured = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	FVector InitialLocation;
	FTimerHandle TimeHandle;
	int32 TimerCounter = 0;


	void PrintTypes();
	void PringStringTypes();
	void PrintTransforms();
	void HandleMovement();
	void MaterialSetup(const FLinearColor &Color, const float &Metallic);
	void TimerStep();
};
