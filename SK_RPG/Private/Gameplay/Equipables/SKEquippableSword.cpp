// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Equipables/SKEquippableSword.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/KismetSystemLibrary.h"

void ASKEquippableSword::BeginPlay()
{
    Super::BeginPlay();

    // SocketsSetup();
}

void ASKEquippableSword::SocketsSetup()
{
    if (!TraceStartSocket || !TraceEndSocket)
    {
        TraceStartSocket = GetStaticMesh()->FindSocket(WeaponBaseSocketName);
        TraceEndSocket = GetStaticMesh()->FindSocket(WeaponTipSocketName);
    }

    ensureMsgf(TraceStartSocket && TraceEndSocket,
               TEXT("Socket(s) are null. Make sure they're set in StaticMesh and have the correct names"));
}

void ASKEquippableSword::HitDetect(FHitResult &HitResult)
{
    if (TraceStartSocket && TraceEndSocket)
    {
        FTransform TraceStart, TraceEnd;
        TraceStartSocket->GetSocketTransform(TraceStart, BaseMesh);
        TraceEndSocket->GetSocketTransform(TraceEnd, BaseMesh);

        if (!SphereTrace(TraceStart.GetLocation(), TraceEnd.GetLocation(), HitResult))
        {
            FillTracingGaps(TraceStart.GetLocation(), TraceEnd.GetLocation(), HitResult);

            PreviousTraceStart = TraceStart.GetLocation();
            PreviousTraceEnd = TraceEnd.GetLocation();
        }
    }
}

void ASKEquippableSword::FillTracingGaps(const FVector &CurrentTraceStart, const FVector &CurrentTraceEnd,
                                         FHitResult &HitResult) const
{
    // Skip if it's the first trace
    if (PreviousTraceStart == FVector::Zero() || PreviousTraceStart == FVector::Zero())
    {
        return;
    }

    const float fpsDelta = FMath::Clamp(1.f / GetWorld()->DeltaRealTimeSeconds, 9.f, 59.f);

    const int32 it = FMath::CeilToInt(FMath::GetMappedRangeValueClamped(TRange<float>::Inclusive(9.f, 60.f),
                                                                        TRange<float>::Inclusive(9.f, 0.f), fpsDelta));
    const float alphaStep = 1.f / it;
    float alpha = alphaStep;

    for (int i = 0; i < it; i++)
    {
        const auto InterpretedStart = FMath::Lerp(PreviousTraceStart, CurrentTraceStart, alpha);
        const auto InterpretedEnd = FMath::Lerp(PreviousTraceEnd, CurrentTraceEnd, alpha);

        alpha += alphaStep;

        SphereTrace(InterpretedStart, InterpretedEnd, HitResult);
        if (HitResult.bBlockingHit) break;
    }

    // UE_LOG(LogTemp, Display, TEXT("Filing gaps"));
}

bool ASKEquippableSword::SphereTrace(const FVector &TraceStart, const FVector &TraceEnd, FHitResult &HitResult) const
{
    ECollisionChannel TraceChannel = ECC_Visibility;

    return UKismetSystemLibrary::SphereTraceSingle(GetWorld(),                                     //
                                                   TraceStart, TraceEnd,                           //
                                                   5.f,                                            // Radius
                                                   UEngineTypes::ConvertToTraceType(TraceChannel), //
                                                   true,                                           // Ignore self
                                                   TArray<AActor *>(),                             // IgnoredActors
                                                   EDrawDebugTrace::None,                          // Debug
                                                   HitResult,                                      //
                                                   true                                            // ignore complex
    );
}

void ASKEquippableSword::ResetTraceStartEnd()
{
    PreviousTraceStart = FVector::Zero();
    PreviousTraceEnd = FVector::Zero();
}

void ASKEquippableSword::InitializeMeleeWeapon(UStaticMesh *MeleeWeaponMesh)
{
    BaseMesh->SetStaticMesh(MeleeWeaponMesh);
    SocketsSetup();
}
