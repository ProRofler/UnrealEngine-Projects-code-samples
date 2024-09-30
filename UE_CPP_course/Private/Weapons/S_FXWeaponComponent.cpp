// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/S_FXWeaponComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

US_FXWeaponComponent::US_FXWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // ImpactFX = CreateDefaultSubobject<UNiagaraSystem>("Impact NS VFX");
}

void US_FXWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
}

void US_FXWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void US_FXWeaponComponent::SpawnEmitter(const FHitResult &HitResult) const
{
    // UE_LOG(LogTemp, Display, TEXT("FX called"));

    auto ImpactFX = DefaultImpactFX;

    if (HitResult.PhysMaterial.IsValid())
    {
        const auto PhysMat = HitResult.PhysMaterial.Get();

        if (FXMap.Contains(PhysMat))
        {
            ImpactFX = FXMap[PhysMat];
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Got the Psysical Material %s but there's not such in the map, please check"),
                   *HitResult.PhysMaterial.Get()->GetName());
        }
    }

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),            //
                                                   ImpactFX.ImpactFX,     //
                                                   HitResult.ImpactPoint, //
                                                   HitResult.ImpactNormal.Rotation());

    const auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),                         //
                                                              ImpactFX.DecalData.Decal,           //
                                                              ImpactFX.DecalData.Size,            //
                                                              HitResult.ImpactPoint,              //
                                                              HitResult.ImpactNormal.Rotation()); //
    if (Decal)
        Decal->SetFadeOut(ImpactFX.DecalData.LifeTime, ImpactFX.DecalData.FadeOutTime);
}
