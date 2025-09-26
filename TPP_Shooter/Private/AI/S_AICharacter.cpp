// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/S_AICharacter.h"
#include "AI/S_AIControllerT.h"
#include "S_AIWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"

AS_AICharacter::AS_AICharacter(const FObjectInitializer &ObjInit)
    : Super(
          ObjInit.SetDefaultSubobjectClass<US_AIWeaponComponent>("Player weapon"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = AS_AIControllerT::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}

void AS_AICharacter::OnDeath()
{
    Super::OnDeath();

    const auto AIController = Cast<AS_AIControllerT>(Controller);
    if(!AIController) return;

    AIController->BrainComponent.Get()->Cleanup();

}
