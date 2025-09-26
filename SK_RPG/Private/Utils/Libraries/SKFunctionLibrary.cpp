// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Utils/Libraries/SKFunctionLibrary.h"

#include "Characters/SKBaseCharacter.h"
#include "Gameplay/GAS/SKNativeGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"

AActor *USKFunctionLibrary::GetLookedAtActor(const TArray<AActor *> &Actors, const FVector &ViewDirection,
                                             const FVector &ViewPoint, const float Distance,
                                             const float DotProductThreshold, const FGameplayTagContainer TagsToIngore)
{
#if !UE_BUILD_SHIPPING
    TRACE_CPUPROFILER_EVENT_SCOPE_STR("USKFunctionLibrary::GetLookedAtActor");
#endif

    double BestDotProduct = -1.0f;
    AActor *BestActor = nullptr;

    for (const auto &Actor : Actors)
    {
        // check distance
        if (Distance > 0.f && FVector::Distance(ViewPoint, Actor->GetActorLocation()) > Distance) continue;

        // Checking if the actor is a character with ASC and currently has any provided tags
        if (!TagsToIngore.IsEmpty())
        {
            if (const auto Character = Cast<ASKBaseCharacter>(Actor))
            {
                const auto ASC = Character->GetAbilitySystemComponent();
                if (ASC && ASC->HasAnyMatchingGameplayTags(TagsToIngore)) continue;
            }
        }

        // calcuate dot product
        const auto DotProduct = FVector::DotProduct(
            ViewDirection, UKismetMathLibrary::GetDirectionUnitVector(ViewPoint, Actor->GetActorLocation()));

        if (DotProduct >= BestDotProduct)
            BestDotProduct = DotProduct;
        else
            continue;

        BestActor = Actor;
    }

    return BestDotProduct < DotProductThreshold ? nullptr : BestActor;
}

FHitResult USKFunctionLibrary::TraceToBoundingBox(const UWorld *World, const FVector TraceStartPoint,
                                                  const AActor *OtherActor)
{
    FHitResult HitResult;

    if (World && OtherActor)
    {
        const auto tracePoint = OtherActor->GetComponentsBoundingBox().GetCenter();

        World->LineTraceSingleByChannel(HitResult, TraceStartPoint, tracePoint, ECollisionChannel::ECC_Visibility);
    }

    return HitResult;
}
