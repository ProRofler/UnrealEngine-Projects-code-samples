// Fill out your copyright notice in the Description page of Project Settings.


#include "GeometryHubActor.h"
#include "Engine/World.h"


DEFINE_LOG_CATEGORY_STATIC(LogTimerOperations, All, All)

// Sets default values
AGeometryHubActor::AGeometryHubActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGeometryHubActor::BeginPlay()
{
	Super::BeginPlay();

	//ObjectsSpawn1();
	//ObjectsSpawn2();
	//ObjectsSpawn3();
	ObjectsSpawn4();

}



// Called every frame
void AGeometryHubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Green, "Property actor pointer: " + FString::FromInt(PropertyActor != nullptr) + " is valid: " + FString::FromInt(IsValid(PropertyActor)), true);
	GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Green, "No property actor pointer: " + FString::FromInt(NoPropertyActor != nullptr) + " is valid: " + FString::FromInt(IsValid(NoPropertyActor)), true);

}

void AGeometryHubActor::ObjectsSpawn1()
{
	UWorld* World = GetWorld();

	if (World)
	{
		for (int32 i = 0; i < ObjectCount; i++)
		{
			const FTransform GeoTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f + (300 * i), 0.0f));
			AS_BaseGeometryActor* Geometry = World->SpawnActor<AS_BaseGeometryActor>(GeometryClass, GeoTransform);

			if (Geometry)
			{
				FGeometryData GeoData;
				GeoData.MoveType = FMath::RandBool() ? EMovementType::Static : EMovementType::Sin;
				Geometry->SetGeo(GeoData);
			}
		}
	}
}

void AGeometryHubActor::ObjectsSpawn2()
{
	UWorld* World = GetWorld();

	if (World)
	{

		for (int32 i = 0; i < ObjectCount; i++)
		{
			const FTransform GeoTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f + (300 * i), 300.0f));
			//SpawnActorDeferred spawns only the construction of a class
			AS_BaseGeometryActor* Geometry = World->SpawnActorDeferred<AS_BaseGeometryActor>(GeometryClass, GeoTransform);

			if (Geometry)
			{
				FMaterialData MatData;
				MatData.Color = FLinearColor::MakeRandomColor();
				Geometry->SetMaterial(MatData);

				//this function calls BeginPLay function
				Geometry->FinishSpawning(GeoTransform);
			}
		}
	}
}

void AGeometryHubActor::ObjectsSpawn3()
{
	UWorld* World = GetWorld();

	if (World)
	{

		for (const FGeometryPayload Payload : GeometryPayloads)
		{

			AS_BaseGeometryActor* Geometry = World->SpawnActorDeferred<AS_BaseGeometryActor>(Payload.GeometryClass, Payload.InitialTransform);

			if (Geometry)
			{
				Geometry->SetGeo(Payload.GeoData);
				Geometry->OnColorChanged.AddDynamic(this, &AGeometryHubActor::OnColorChanged);
				Geometry->OnTimerFinished.AddUObject(this, &AGeometryHubActor::OnTimerFinished);
				Geometry->FinishSpawning(Payload.InitialTransform);
			}

		}

	}
}

void AGeometryHubActor::ObjectsSpawn4()
{
	if (!GetWorld()) return;

	const FTransform GeoTransform = FTransform();
	PropertyActor = GetWorld()->SpawnActorDeferred<AS_BaseGeometryActor>(GeometryClass, GeoTransform);

	NoPropertyActor = GetWorld()->SpawnActorDeferred<AS_BaseGeometryActor>(GeometryClass, GeoTransform);

}

void AGeometryHubActor::OnColorChanged(const FLinearColor& Color, const FString& Name)
{
	UE_LOG(LogTimerOperations, Display, TEXT("DELEGATE WORK Actor name: %s Color %s"), *Name, *Color.ToString());
}

void AGeometryHubActor::OnTimerFinished(AActor* Actor) //В данном случае происходит апкастинг
{
	if (!Actor) return;
	UE_LOG(LogTimerOperations, Warning, TEXT("DELEGATE WORK Actor name: %s timer is finished"), *Actor->GetName());


	//преобразуем полученный родительский актор в наш класс актора
	AS_BaseGeometryActor* Geometry = Cast<AS_BaseGeometryActor> (Actor);
	if (!Geometry) return;

	UE_LOG(LogTimerOperations, Warning, TEXT("DELEGATE WORK Cast is success. Passing amplitude %f"), Geometry->GetGeoData().amplitude);

	Geometry->Destroy();
	//Geometry->SetLifeSpan(2.0f);

}	

