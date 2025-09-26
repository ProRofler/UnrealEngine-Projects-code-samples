// Fill out your copyright notice in the Description page of Project Settings.


#include "S_TutorialPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogPawnOperations, All, All)

// Sets default values
AS_TutorialPawn::AS_TutorialPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create scene component
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene component");
	SetRootComponent(SceneComponent);


	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Player camera");
	CameraComponent -> SetupAttachment(GetRootComponent());

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("PlayerMesh");
	StaticMeshComponent->SetupAttachment(GetRootComponent());


}

// Called when the game starts or when spawned
void AS_TutorialPawn::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AS_TutorialPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!VelocityVector.IsZero())
	{
		const FVector MoveUpdate = GetActorLocation() + Velocity * DeltaTime * VelocityVector;
		SetActorLocation(MoveUpdate);
		VelocityVector = FVector::ZeroVector;
	}

}

// Called to bind functionality to input
void AS_TutorialPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &AS_TutorialPawn::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AS_TutorialPawn::MoveRight);
	}
	

}

void AS_TutorialPawn::MoveForward(float Amount)
{
	VelocityVector.X = Amount;
	FString MoveVector = FString::Printf(TEXT("Forvard vector: %f"), Amount);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, MoveVector, true);
	
}

void AS_TutorialPawn::MoveRight(float Amount)
{
	//if (Amount)
	VelocityVector.Y = Amount;
	FString MoveVector = FString::Printf(TEXT("Right vector: %f"), Amount);
	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green, MoveVector, true);
	
}

void AS_TutorialPawn::PossessedBy(AController* CurrentController)
{
	Super::PossessedBy(CurrentController);

	if (!CurrentController) return;
	UE_LOG(LogPawnOperations, Warning, TEXT("%s now possesed by %s"), *GetName(), *CurrentController -> GetName());
}

void AS_TutorialPawn::UnPossessed()
{
	Super::UnPossessed();
	UE_LOG(LogPawnOperations, Warning, TEXT("%s is now unpossesed"), *GetName());

}

