// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/SKCommonGameplayTagsLib.h"
#include "GameplayTagsManager.h"

FGameplayTag USKCommonGameplayTagsLib::WantsToSprintTag;
FGameplayTag USKCommonGameplayTagsLib::WantsToJumpTag;

FGameplayTag USKCommonGameplayTagsLib::WantsToAttack;

FGameplayTag USKCommonGameplayTagsLib::GrabbingItemTag;
FGameplayTag USKCommonGameplayTagsLib::RotatingItemTag;
FGameplayTag USKCommonGameplayTagsLib::BlockingTag;

FGameplayTag USKCommonGameplayTagsLib::IdlingTag;
FGameplayTag USKCommonGameplayTagsLib::RunningTag;
FGameplayTag USKCommonGameplayTagsLib::WalkingTag;
FGameplayTag USKCommonGameplayTagsLib::SprintingTag;
FGameplayTag USKCommonGameplayTagsLib::InAirTag;

FGameplayTag USKCommonGameplayTagsLib::WeaponDrawnTag;

FGameplayTag USKCommonGameplayTagsLib::CanInteractTag;
FGameplayTag USKCommonGameplayTagsLib::PerformingActionTag;

USKCommonGameplayTagsLib::USKCommonGameplayTagsLib()
{
    WantsToSprintTag = FGameplayTag::RequestGameplayTag("Character.Request.Movement.WantsToSprint");
    WantsToJumpTag = FGameplayTag::RequestGameplayTag("Character.Request.Movement.WantsToJump");

    WantsToAttack = FGameplayTag::RequestGameplayTag("Character.Request.Combat.WantsToAttack");

    GrabbingItemTag = FGameplayTag::RequestGameplayTag("Character.State.Action.GrabbingItem");
    RotatingItemTag = FGameplayTag::RequestGameplayTag("Character.State.Action.RotatingItem");
    BlockingTag = FGameplayTag::RequestGameplayTag("Character.State.Combat.Blocking");

    IdlingTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.Idling");
    RunningTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.Running");
    WalkingTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.Walking");
    SprintingTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.Sprinting");
    InAirTag = FGameplayTag::RequestGameplayTag("Character.State.Movement.InAir");

    WeaponDrawnTag = FGameplayTag::RequestGameplayTag("Character.State.Combat.WeaponDrawn");

    CanInteractTag = FGameplayTag::RequestGameplayTag("Character.State.Action.CanInteract");
    PerformingActionTag = FGameplayTag::RequestGameplayTag("Character.State.Action.PerformingAction");
}

const FGameplayTag &USKCommonGameplayTagsLib::GetTag_WantsToSprint() { return WantsToSprintTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_WantsToJump() { return WantsToJumpTag; }

const FGameplayTag &USKCommonGameplayTagsLib::GetTag_WantsToAttack() { return WantsToAttack; }

const FGameplayTag &USKCommonGameplayTagsLib::GetTag_GrabbingItem() { return GrabbingItemTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_RotatingItem() { return RotatingItemTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Blocking() { return BlockingTag; }

const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Idling() { return IdlingTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Running() { return RunningTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Walking() { return WalkingTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_Sprinting() { return SprintingTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_InAir() { return InAirTag; }

const FGameplayTag &USKCommonGameplayTagsLib::GetTag_WeaponDrawn() { return WeaponDrawnTag; }

const FGameplayTag &USKCommonGameplayTagsLib::GetTag_CanInteract() { return CanInteractTag; }
const FGameplayTag &USKCommonGameplayTagsLib::GetTag_PerformingAction() { return PerformingActionTag; }
