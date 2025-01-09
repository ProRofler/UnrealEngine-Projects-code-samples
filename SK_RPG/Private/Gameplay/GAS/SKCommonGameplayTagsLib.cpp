// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/SKCommonGameplayTagsLib.h"
#include "GameplayTagsManager.h"


FGameplayTag USKCommonGameplayTagsLib::GrabbingItemTag;
FGameplayTag USKCommonGameplayTagsLib::RotatingItemTag;
FGameplayTag USKCommonGameplayTagsLib::WantsToSprintTag;
FGameplayTag USKCommonGameplayTagsLib::IdlingTag;
FGameplayTag USKCommonGameplayTagsLib::RunningTag;
FGameplayTag USKCommonGameplayTagsLib::WalkingTag;
FGameplayTag USKCommonGameplayTagsLib::CanInteractTag;


USKCommonGameplayTagsLib::USKCommonGameplayTagsLib()
{
    GrabbingItemTag = FGameplayTag::RequestGameplayTag("Character.State.Action.GrabbingItem");
    RotatingItemTag = FGameplayTag::RequestGameplayTag("Character.State.Action.RotatingItem");
    WantsToSprintTag = FGameplayTag::RequestGameplayTag("Character.Request.Movement.WantsToSprint");
    IdlingTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.Idling");
    RunningTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.Running");
    WalkingTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.Walking");
    CanInteractTag = FGameplayTag::RequestGameplayTag("Character.State.Action.CanInteract");
}

const FGameplayTag &USKCommonGameplayTagsLib::GetTag_GrabbingItem()
{
    return GrabbingItemTag;
}
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_RotatingItem()
{
    return RotatingItemTag;
}
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_WantsToSprint()
{
    return WantsToSprintTag;
}
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Idling()
{
    return IdlingTag;
}
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Running()
{
    return RunningTag;
}
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Walking()
{
    return WalkingTag;
}
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_CanInteract()
{
    return CanInteractTag;
}
