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

    static const FGameplayTag &GetTag_WantsToSprint();
    static const FGameplayTag &GetTag_WantsToJump();

    static const FGameplayTag &GetTag_WantsToAttack();

    static const FGameplayTag &GetTag_GrabbingItem();
    static const FGameplayTag &GetTag_RotatingItem();
    static const FGameplayTag &GetTag_Blocking();

    static const FGameplayTag &GetTag_Idling();
    static const FGameplayTag &GetTag_Running();
    static const FGameplayTag &GetTag_Sprinting();
    static const FGameplayTag &GetTag_Walking();
    static const FGameplayTag &GetTag_InAir();

    static const FGameplayTag &GetTag_WeaponDrawn();

    static const FGameplayTag &GetTag_CanInteract();
    static const FGameplayTag &GetTag_PerformingAction();

  private:
    static FGameplayTag WantsToSprintTag;
    static FGameplayTag WantsToJumpTag;

    static FGameplayTag WantsToAttack;

    static FGameplayTag GrabbingItemTag;
    static FGameplayTag RotatingItemTag;
    static FGameplayTag BlockingTag;

    static FGameplayTag IdlingTag;
    static FGameplayTag RunningTag;
    static FGameplayTag SprintingTag;
    static FGameplayTag WalkingTag;
    static FGameplayTag InAirTag;

    static FGameplayTag WeaponDrawnTag;

    static FGameplayTag CanInteractTag;
    static FGameplayTag PerformingActionTag;
};
