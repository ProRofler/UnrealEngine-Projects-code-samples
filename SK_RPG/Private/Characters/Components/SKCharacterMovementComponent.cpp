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

        SetCharacterSpeed(EMovementType::ERunning);
}

void USKCharacterMovementComponent::SetCharacterSpeed(const EMovementType& MovementType)
{
    switch (MovementType)
    {
    case EMovementType::EWalking:
        MaxWalkSpeed = 120.0f;
        break;
    case EMovementType::ERunning:
        MaxWalkSpeed = BaseWalkSpeed * (Athletics / 100.0f);
        break;
    case EMovementType::ESprinting:
        MaxWalkSpeed = (BaseWalkSpeed * 2.0f) * (Athletics / 100.0f);
        break;
    default:
        UE_LOG(LogSKCharacterMovement, Error, TEXT("Invalid or not implemented enum EMovementType was passed in %s Movement component's SetCharacterSpeed funtion"), *GetOwner()->GetActorNameOrLabel());
        checkf(false, TEXT("Invalid or not implemented enum EMovementType was passed"));
        break;
    }
}
