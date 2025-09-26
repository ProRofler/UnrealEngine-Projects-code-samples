// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S_BaseGeometryActor.h"
#include "GeometryHubActor.generated.h"


USTRUCT(BlueprintType)
struct FGeometryPayload
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TSubclassOf<AS_BaseGeometryActor> GeometryClass;

    UPROPERTY(EditAnywhere)
    FGeometryData GeoData;

    UPROPERTY(EditAnywhere)
    FTransform InitialTransform;
};

UCLASS()
class CPP_02_API AGeometryHubActor : public AActor
{
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    AGeometryHubActor();

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AS_BaseGeometryActor> GeometryClass;

    /*
    UPROPERTY(EditAnywhere)
    UClass* Class;

    UPROPERTY(EditAnywhere)
    AS_BaseGeometryActor* GeometryObject;
    */

    UPROPERTY(EditAnywhere)
    int32 ObjectCount = 5;

    UPROPERTY(EditAnywhere)
    TArray<FGeometryPayload> GeometryPayloads;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

  private:
    void ObjectsSpawn1();
    void ObjectsSpawn2();
    void ObjectsSpawn3();
    void ObjectsSpawn4();

    UPROPERTY()
    AS_BaseGeometryActor *PropertyActor;
    UPROPERTY()
    AS_BaseGeometryActor *NoPropertyActor;

    UFUNCTION() // must be used for a dynamic delegates
    void OnColorChanged(const FLinearColor &Color, const FString &Name);

    void OnTimerFinished(AActor *Actor);
};
