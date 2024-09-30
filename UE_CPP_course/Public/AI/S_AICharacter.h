// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/S_PLayerCharacter.h"
#include "S_AICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class CPP_02_API AS_AICharacter : public AS_PLayerCharacter
{
    GENERATED_BODY()
    
public:
    AS_AICharacter(const FObjectInitializer &ObjInit);

    UBehaviorTree* GetBehaviorTree() {return DefaultBehaviorTree;};

protected:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree * DefaultBehaviorTree = nullptr;

    virtual void OnDeath() override;

};
