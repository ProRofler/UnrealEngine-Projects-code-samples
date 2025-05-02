// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/GAS/SKNativeGameplayTags.h"
#include "GameplayTagsManager.h"

FSKGameplayTags FSKGameplayTags::GameplayTags;

void FSKGameplayTags::InitializeNativeGameplayTags()
{
    /*
     * Primary character action states
     */

    GameplayTags.Character_State_Action_CanInteract =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Action.CanInteract"));

    GameplayTags.Character_State_Action_GrabbingItem =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Action.GrabbingItem"));

    GameplayTags.Character_State_Action_Interacting =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Action.Interacting"));

    GameplayTags.Character_State_Action_PerformingAction =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Action.PerformingAction"));

    GameplayTags.Character_State_Action_RotatingItem =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Action.RotatingItem"));

    /*
     * Primary character combat states
     */

    GameplayTags.Character_State_Combat_Attacking =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Combat.Attacking"));

    GameplayTags.Character_State_Combat_ActiveAttacking =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Combat.ActiveAttacking"));

    GameplayTags.Character_State_Combat_Blocking =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Combat.Blocking"));

    GameplayTags.Character_State_Combat_WeaponUnsheathed =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Combat.WeaponUnsheathed"));

    /*
     * Primary character movement states
     */

    GameplayTags.Character_State_Movement_Idling =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Movement.Idling"));

    GameplayTags.Character_State_Movement_Crouching =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Movement.Crouching"));

    GameplayTags.Character_State_Movement_InAir =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Movement.InAir"));

    GameplayTags.Character_State_Movement_Running =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Movement.Running"));

    GameplayTags.Character_State_Movement_Sprinting =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Movement.Sprinting"));

    GameplayTags.Character_State_Movement_Walking =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Movement.Walking"));

    /*
     * Main attributes
     */

    GameplayTags.Attibute_Main_Health =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Main.Health"));

    GameplayTags.Attibute_Main_Stamina =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Main.Stamina"));

    /*
     * Gameplay events
     */

    // stamina
    GameplayTags.Event_Attibute_Decrease_Stamina =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attribute.Decrease.Stamina"));

    GameplayTags.Event_Attibute_Increase_Stamina =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attribute.Increase.Stamina"));

    GameplayTags.Event_Attibute_Depleted_Stamina =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attribute.Depleted.Stamina"));

    GameplayTags.Event_Attibute_Full_Stamina =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attribute.Full.Stamina"));

    // health
    GameplayTags.Event_Attibute_Decrease_Health =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attribute.Decrease.Health"));

    GameplayTags.Event_Attibute_Increase_Health =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attribute.Increase.Health"));

    GameplayTags.Event_Attibute_Depleted_Health =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attribute.Depleted.Health"));

    GameplayTags.Event_Attibute_Full_Health =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attribute.Full.Health"));

    // Ability activation
    GameplayTags.Event_Ability_UseItem =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Ability.UseItem"));

    // Misc
    GameplayTags.Event_Combat_SwitchWeapon =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Combat.SwitchWeapon"));

    GameplayTags.Event_Combat_Hit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Combat.Hit"));

    /*
     * Gameplay effects
     */

    GameplayTags.Effect_Data_FallDamage =
        UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.Data.FallDamage"));
}
