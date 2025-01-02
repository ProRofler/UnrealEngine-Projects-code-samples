// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKStateMachineComponent.h"

USKStateMachineComponent::USKStateMachineComponent() { PrimaryComponentTick.bCanEverTick = false; }

void USKStateMachineComponent::RequestAction(const FGameplayTag &ActionRequestTag)
{
    // TODO: Add a tag to container and run the logic
}

void USKStateMachineComponent::BeginPlay()
{
    Super::BeginPlay();
    //ActionState = FGameplayTag::RequestGameplayTag(FName("Character.ActionType.None"));
    //MovementState = FGameplayTag::RequestGameplayTag(FName("Character.MovementType.Running"));
}
