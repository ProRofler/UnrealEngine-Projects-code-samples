// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKCharacterMovementComponent.h"
#include "Characters/SKBaseCharacter.h"
#include "Core/SKLogCategories.h"

USKCharacterMovementComponent::USKCharacterMovementComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void USKCharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    Character = Cast<ASKBaseCharacter>(GetOwner());
    check(Character);

    BaseWalkSpeed = MaxWalkSpeed;
    check(BaseWalkSpeed == MaxWalkSpeed)
}

void USKCharacterMovementComponent::StartRunning() { MaxWalkSpeed = BaseWalkSpeed * (Athletics / 100.0f); }

void USKCharacterMovementComponent::StartSprinting() { MaxWalkSpeed = (BaseWalkSpeed * 2.0f) * (Athletics / 100.0f); }

void USKCharacterMovementComponent::StartWalking() { MaxWalkSpeed = 120.0f;  }
