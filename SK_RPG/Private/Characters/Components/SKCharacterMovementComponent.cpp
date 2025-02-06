// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKCharacterMovementComponent.h"
#include "Characters/SKBaseCharacter.h"
#include "Core/SKLogCategories.h"
#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Gameplay/GAS/SKAttributeSetSkills.h"
#include "Gameplay/GAS/SKCommonGameplayTagsLib.h"

USKCharacterMovementComponent::USKCharacterMovementComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void USKCharacterMovementComponent::BeginPlay()
{
    OwnerSKCharacter = Cast<ASKBaseCharacter>(GetOwner());
    check(OwnerSKCharacter.IsValid());

    AttributeSetSkills = OwnerSKCharacter->GetAbilitySystemComponent()->GetSet<USKAttributeSetSkills>();
    check(AttributeSetSkills);

    BaseWalkSpeed = MaxWalkSpeed;
    check(BaseWalkSpeed == MaxWalkSpeed)

        Super::BeginPlay();
}

void USKCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    HandleRunningSpeed();
}

void USKCharacterMovementComponent::StartRunning()
{
    FScopeLock Lock(&CriticalSection);

    MaxWalkSpeed = BaseWalkSpeed * (AttributeSetSkills->GetAthletics() / 100.0f);
}

void USKCharacterMovementComponent::StartSprinting()
{
    FScopeLock Lock(&CriticalSection);

    MaxWalkSpeed = (BaseWalkSpeed * 2.0f) * (AttributeSetSkills->GetAthletics() / 100.0f);
}

void USKCharacterMovementComponent::StartWalking()
{
    FScopeLock Lock(&CriticalSection);

    MaxWalkSpeed = 120.0f;
}

void USKCharacterMovementComponent::HandleRunningSpeed()
{
    const auto runningTagC = USKCommonGameplayTagsLib::GetTag_Running().GetSingleTagContainer();
    if (OwnerSKCharacter->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(runningTagC))
    {
        Async(EAsyncExecution::TaskGraph, [&]() {
            FScopeLock Lock(&CriticalSection);

            auto decreaseCoef = FMath::GetMappedRangeValueClamped(
                TRange<float>::Inclusive(0.0f, 180.0f), TRange<float>::Inclusive(1.0f, 0.3f),
                FMath::Abs(OwnerSKCharacter->GetCharacterMovementAngle()));

            MaxWalkSpeed = BaseWalkSpeed * decreaseCoef;
        });
    }
}
