// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/S_WeaponSwitchNotify.h"

void US_WeaponSwitchNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation)
{
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}
