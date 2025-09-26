// Fill out your copyright notice in the Description page of Project Settings.


#include "S_BaseGeometryActor.h"
#include "Engine\Engine.h"
#include "Materials\MaterialInstanceDynamic.h"
#include "TimerManager.h"


DEFINE_LOG_CATEGORY_STATIC(LogBaseGeometry, All, All)
DEFINE_LOG_CATEGORY_STATIC(LogMaterialUpdate, All, All)

// Sets default values
AS_BaseGeometryActor::AS_BaseGeometryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

}

// Called when the game starts or when spawned
void AS_BaseGeometryActor::BeginPlay()
{
	Super::BeginPlay();

	MaterialSetup(MaterialData.Color, MaterialData.Metallic);


	InitialLocation = GetActorLocation();
	
	GetWorldTimerManager().SetTimer(TimeHandle, this, &AS_BaseGeometryActor::TimerStep, MaterialData.TimerAmount, true);

	//PrintTransforms();
	//PrintTypes();
	//PringStringTypes();

}

void AS_BaseGeometryActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);

	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name: %s is destroyed"), *GetName());

}

// Called every frame
void AS_BaseGeometryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement();

}

void AS_BaseGeometryActor::PrintTypes()
{	
	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name: %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Ammo count: %d, DeathAmount: %i"), AmmoCount, DeathAmount);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Full health amount: %f"), Health);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Double health amount: %.2f"), Health);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Is falling %d"), IsFalling);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Is injured %d"), static_cast<int>(IsInjured));
}

void AS_BaseGeometryActor::PringStringTypes()
{
	FString name = "Eden Hazard";

	int CreditsCount = 25;
	float Armor = 78.255685f;
	bool IsDead = false;

	UE_LOG(LogBaseGeometry, Display, TEXT("Name is: %s"), *name);

	FString CreditsStr = "Credits count: " + FString::FromInt(CreditsCount);
	FString ArmorStr = "Health amount: " + FString::SanitizeFloat(Armor);
	FString DeadStr = "Is currently falling: " + FString(IsDead ? "False" : "True");


	FString Stats = FString::Printf(TEXT("==All stats : ==\n%s\n%s\n%s"), *CreditsStr, *ArmorStr, *DeadStr);

	if (GEngine)
	{
		UE_LOG(LogBaseGeometry, Warning, TEXT("%s"), *Stats);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, name);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Stats, true, FVector2D(2.0f, 2.0f));
	}
	
}

void AS_BaseGeometryActor::PrintTransforms()
{
	FTransform transform = GetActorTransform();
	FVector location = GetActorLocation();
	FRotator rotation = GetActorRotation();
	FVector scale = GetActorScale3D();


	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name: %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Transform: %s"), *transform.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Location: %s"), *location.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Rotation: %s"), *rotation.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Scale: %s"), *scale.ToString());


	UE_LOG(LogBaseGeometry, Warning, TEXT("Human readable string transform:\n%s"), *transform.ToHumanReadableString());
}

void AS_BaseGeometryActor::HandleMovement()
{

	switch (GeometryData.MoveType)
	{
	case EMovementType::Sin:
	{
		FVector CurrentLocation = GetActorLocation();
		if (GetWorld())
		{
			float WTime = GetWorld()->GetTimeSeconds();
			CurrentLocation.Z = InitialLocation.Z + GeometryData.amplitude * FMath::Sin(GeometryData.frequency * WTime);

			SetActorLocation(CurrentLocation);
		}
		
	}
	break;
	case EMovementType::Static:break;
	default:break;
	}

}

void AS_BaseGeometryActor::MaterialSetup(const FLinearColor& Color, const float& Metallic)
{

	//check if BaseMesh pointer is not null. If it is, immidiately return
	if (!BaseMesh) return;

	UMaterialInstanceDynamic* DynMat = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);

	if (DynMat)
	{
		DynMat->SetVectorParameterValue("Albedo", Color);
		DynMat->SetScalarParameterValue("Metallic", Metallic);
	}

}

void AS_BaseGeometryActor::TimerStep()
{	
	if (++TimerCounter <= MaterialData.TimerStep)
	{
		const FLinearColor NewColor = FLinearColor::MakeRandomColor();
		const float Metallic = MaterialData.Metallic;
		UE_LOG(LogMaterialUpdate, Warning, TEXT("Step number: %i New color is: %s"), TimerCounter, *NewColor.ToString());
		MaterialSetup(NewColor, Metallic);
		OnColorChanged.Broadcast(NewColor, GetName());
	}
	else
	{
		UE_LOG(LogMaterialUpdate, Warning, TEXT("Timer has stopped"));
		GetWorldTimerManager().ClearTimer(TimeHandle);
		OnTimerFinished.Broadcast(this); //передаем указатель на текущий актор
	}
	

}

