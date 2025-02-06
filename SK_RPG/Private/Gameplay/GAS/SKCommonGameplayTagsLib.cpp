// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/SKCommonGameplayTagsLib.h"
#include "GameplayTagsManager.h"

FGameplayTag USKCommonGameplayTagsLib::GrabbingItemTag;
FGameplayTag USKCommonGameplayTagsLib::RotatingItemTag;
FGameplayTag USKCommonGameplayTagsLib::WantsToSprintTag;
FGameplayTag USKCommonGameplayTagsLib::WantsToJumpTag;
FGameplayTag USKCommonGameplayTagsLib::IdlingTag;
FGameplayTag USKCommonGameplayTagsLib::RunningTag;
FGameplayTag USKCommonGameplayTagsLib::WalkingTag;
FGameplayTag USKCommonGameplayTagsLib::CanInteractTag;
FGameplayTag USKCommonGameplayTagsLib::WeaponDrawnTag;
FGameplayTag USKCommonGameplayTagsLib::SprintingTag;
FGameplayTag USKCommonGameplayTagsLib::InAirTag;

USKCommonGameplayTagsLib::USKCommonGameplayTagsLib()
{
    GrabbingItemTag = FGameplayTag::RequestGameplayTag("Character.State.Action.GrabbingItem");
    RotatingItemTag = FGameplayTag::RequestGameplayTag("Character.State.Action.RotatingItem");
    WantsToSprintTag = FGameplayTag::RequestGameplayTag("Character.Request.Movement.WantsToSprint");
    WantsToJumpTag = FGameplayTag::RequestGameplayTag("Character.Request.Movement.WantsToJump");
    IdlingTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.Idling");
    RunningTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.Running");
    WalkingTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.Walking");
    InAirTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.InAir");
    CanInteractTag = FGameplayTag::RequestGameplayTag("Character.State.Action.CanInteract");
    WeaponDrawnTag = FGameplayTag::RequestGameplayTag("Character.State.Action.WeaponDrawn");
    SprintingTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.Sprinting");
}

const FGameplayTag &USKCommonGameplayTagsLib::GetTag_GrabbingItem() { return GrabbingItemTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_RotatingItem() { return RotatingItemTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_WantsToSprint() { return WantsToSprintTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_WantsToJump() { return WantsToJumpTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Idling() { return IdlingTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Running() { return RunningTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_InAir() { return InAirTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Walking() { return WalkingTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_CanInteract() { return CanInteractTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Sprinting() { return SprintingTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_WeaponDrawn() { return WeaponDrawnTag; }
