// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"

#include "SKCommonGameplayTagsLib.generated.h"

UCLASS()
class SIRKNIGHT_API USKCommonGameplayTagsLib : public UObject
{
    GENERATED_BODY()

  public:
    USKCommonGameplayTagsLib();

    static const FGameplayTag &GetTag_GrabbingItem();
    static const FGameplayTag &GetTag_RotatingItem();
    static const FGameplayTag &GetTag_WantsToSprint();
    static const FGameplayTag &GetTag_Idling();
    static const FGameplayTag &GetTag_Running();
    static const FGameplayTag &GetTag_Walking();
    static const FGameplayTag &GetTag_CanInteract();

  private:
    static FGameplayTag GrabbingItemTag;
    static FGameplayTag RotatingItemTag;
    static FGameplayTag WantsToSprintTag;
    static FGameplayTag IdlingTag;
    static FGameplayTag RunningTag;
    static FGameplayTag WalkingTag;
    static FGameplayTag CanInteractTag;
};
