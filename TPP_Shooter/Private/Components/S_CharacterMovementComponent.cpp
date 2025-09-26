// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/S_CharacterMovementComponent.h"
#include "Player/S_PLayerCharacter.h"

float US_CharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const AS_PLayerCharacter *PLayer = Cast<AS_PLayerCharacter>(GetPawnOwner());

   
    if (PLayer->bWantsToRun)
    {
        return Speed * SprintModifier;
    }
    
    return Speed;
   
}
