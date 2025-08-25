// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SKFunctionLibrary.generated.h"

class ASKBaseCharacter;

UCLASS()
class SIRKNIGHT_API USKFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintPure, Category = "SK Statics library | Animation Utils")
    static FName GetMontageSectionNameByIndex(const UAnimMontage *Montage, const int32 Index);

    static FHitResult TraceToBoundingBox(const UWorld *World, const FVector TraceStartPoint, const AActor *OtherActor);
};
