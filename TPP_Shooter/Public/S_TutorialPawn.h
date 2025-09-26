// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "S_TutorialPawn.generated.h"

//class UInputMappingContext;
//class UInputAction;

class UCameraComponent;


UCLASS()
class CPP_02_API AS_TutorialPawn : public APawn
{
	GENERATED_BODY()

private:

	FVector VelocityVector = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	//на axis маппиги одна сигнатура
	void MoveForward(float Amount);
	void MoveRight(float Amount);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**	UPROPERTY(EditAnywhere, Category = "User input")
		UInputMappingContext* MoveContext;

		UPROPERTY(EditAnywhere, Category = "User input")
		UInputAction* MoveAction;
	*/

public:
	// Sets default values for this pawn's properties
	AS_TutorialPawn();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	float Velocity = 300.f;

	virtual void PossessedBy(AController* CurrentController);
	virtual void UnPossessed();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced input");
	//UInputMappingContext 

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
