// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SKFunctionLibrary.generated.h"

class ASKBaseCharacter;

UCLASS()
class SIRKNIGHT_API USKFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    // clang-format off
    UFUNCTION(BlueprintPure, Category = "SK Functions library | Utils",
              meta = (ToolTip = "Returns currently looked at actor from a provided view direction. Distance value of 0 and below will be ignored, as well as empty tag container"))
   // clang-format on    
static AActor *GetLookedAtActor(const TArray<AActor *> &Actors, const FVector &ViewDirection, const FVector &ViewPoint, const float Distance = 0.f,
    const float DotProductThreshold = 0.9f,
    const FGameplayTagContainer TagsToIngore = FGameplayTagContainer());

    static FHitResult TraceToBoundingBox(const UWorld *World, const FVector TraceStartPoint, const AActor *OtherActor);
};
