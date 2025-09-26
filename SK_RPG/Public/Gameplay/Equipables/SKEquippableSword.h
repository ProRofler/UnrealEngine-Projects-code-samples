// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Equipables/SKEquippableBase.h"

#include "SKEquippableSword.generated.h"

UCLASS()
class SIRKNIGHT_API ASKEquippableSword : public ASKEquippableBase
{
    GENERATED_BODY()

  public:
    void HitDetect(FHitResult &HitResult);
    void ResetTraceStartEnd();

    void InitializeMeleeWeapon(UStaticMesh *MeleeWeaponMesh);

  protected:
    virtual void BeginPlay() override;

    FName WeaponBaseSocketName = "WeaponBase";
    UPROPERTY()
    UStaticMeshSocket *TraceStartSocket;

    FName WeaponTipSocketName = "WeaponTip";
    UPROPERTY()
    UStaticMeshSocket *TraceEndSocket;

  private:
    void SocketsSetup();
    bool SphereTrace(const FVector &TraceStart, const FVector &TraceEnd, FHitResult &HitResult) const;
    void FillTracingGaps(const FVector &CurrentTraceStart, const FVector &CurrentTraceEnd, FHitResult &HitResult) const;

    FVector PreviousTraceStart, PreviousTraceEnd;
};
