// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * SirKnightGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FSKGameplayTags
{
  public:
    static const FSKGameplayTags &Get() { return GameplayTags; }
    static void InitializeNativeGameplayTags();

    FGameplayTag Character_State_Action_CanInteract;
    FGameplayTag Character_State_Action_GrabbingItem;
    FGameplayTag Character_State_Action_Interacting;
    FGameplayTag Character_State_Action_PerformingAction;
    FGameplayTag Character_State_Action_RotatingItem;

    FGameplayTag Character_State_Combat_Attacking;
    FGameplayTag Character_State_Combat_Blocking;
    FGameplayTag Character_State_Combat_WeaponUnsheathed;

    FGameplayTag Character_State_Movement_Idling;
    FGameplayTag Character_State_Movement_Crouching;
    FGameplayTag Character_State_Movement_InAir;
    FGameplayTag Character_State_Movement_Running;
    FGameplayTag Character_State_Movement_Sprinting;
    FGameplayTag Character_State_Movement_Walking;

    FGameplayTag Attibute_Main_Stamina;
    FGameplayTag Attibute_Main_Health;

    FGameplayTag Event_Attibute_Increase_Stamina;
    FGameplayTag Event_Attibute_Decrease_Stamina;
    FGameplayTag Event_Attibute_Depleted_Stamina;
    FGameplayTag Event_Attibute_Full_Stamina;

    FGameplayTag Event_Attibute_Increase_Health;
    FGameplayTag Event_Attibute_Decrease_Health;
    FGameplayTag Event_Attibute_Depleted_Health;
    FGameplayTag Event_Attibute_Full_Health;

    FGameplayTag Event_Ability_UseItem;

    FGameplayTag Effect_Data_FallDamage;

  private:
    static FSKGameplayTags GameplayTags;
};
