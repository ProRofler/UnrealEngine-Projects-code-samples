// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CoreMinimal.h"
#include "S_WeaponSwitchNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

UCLASS()
class CPP_02_API US_WeaponSwitchNotify : public UAnimNotify
{
    GENERATED_BODY()

  public:
    virtual void Notify(class USkeletalMeshComponent *MeshComp, class UAnimSequenceBase *Animation) override;

    FOnNotifiedSignature OnNotified;

};
