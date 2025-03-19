// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKCharacterMovementComponent.h"
#include "Characters/Components/SKCharacterComponentBase.h"

#include "Characters/SKBaseCharacter.h"

#include "Gameplay/GAS/SKAbilitySystemComponent.h"
#include "Gameplay/GAS/SKAttributeSetSkills.h"
#include "Gameplay/GAS/SKCommonGameplayTagsLib.h"

#include "Core/SKLogCategories.h"

USKCharacterMovementComponent::USKCharacterMovementComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
    SKCharacterComponentBase = CreateDefaultSubobject<USKCharacterComponentBase>("SK Character component base");
}

void USKCharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    BaseWalkSpeed = MaxWalkSpeed;
    check(BaseWalkSpeed == MaxWalkSpeed)
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

    MaxWalkSpeed =
        BaseWalkSpeed *
        (GetSKOwnerCharacter()->GetAbilitySystemComponent()->GetSet<USKAttributeSetSkills>()->GetAthletics() / 100.0f);
}

void USKCharacterMovementComponent::StartSprinting()
{
    FScopeLock Lock(&CriticalSection);

    MaxWalkSpeed =
        (BaseWalkSpeed * 2.0f) *
        (GetSKOwnerCharacter()->GetAbilitySystemComponent()->GetSet<USKAttributeSetSkills>()->GetAthletics() / 100.0f);
}

void USKCharacterMovementComponent::StartWalking()
{
    FScopeLock Lock(&CriticalSection);

    MaxWalkSpeed = 120.0f;
}

ASKBaseCharacter *USKCharacterMovementComponent::GetSKOwnerCharacter() const
{
    return SKCharacterComponentBase->GetSKOwnerCharacter();
}

void USKCharacterMovementComponent::HandleRunningSpeed()
{
    const auto runningTagC = USKCommonGameplayTagsLib::GetTag_Running().GetSingleTagContainer();
    if (GetSKOwnerCharacter()->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(runningTagC))
    {
        Async(EAsyncExecution::TaskGraph, [&]() {
            FScopeLock Lock(&CriticalSection);

            auto decreaseCoef = FMath::GetMappedRangeValueClamped(
                TRange<float>::Inclusive(0.0f, 180.0f), TRange<float>::Inclusive(1.0f, 0.3f),
                FMath::Abs(GetSKOwnerCharacter()->GetCharacterMovementAngle()));

            const auto runSpeedAdjust =
                GetSKOwnerCharacter()->GetAbilitySystemComponent()->GetSet<USKAttributeSetSkills>()->GetAthletics() /
                100.0f;

            MaxWalkSpeed = BaseWalkSpeed * runSpeedAdjust * decreaseCoef;
        });
    }
}
