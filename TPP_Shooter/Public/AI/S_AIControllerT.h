// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "S_AIControllerT.generated.h"


class US_AISightPerceptionComponent;


UCLASS()
class CPP_02_API AS_AIControllerT : public AAIController
{
    GENERATED_BODY()

  public:
 
    AS_AIControllerT();

protected:
    
    virtual void OnPossess(APawn *InPawn) override;
    virtual void Tick(float DeltaTime) override;
 
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI Components")
    US_AISightPerceptionComponent * SightPerceptionComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    FName FocusOnName = "EnemyActor";


private:
    AActor* GetFocusOnActor() const;
};
