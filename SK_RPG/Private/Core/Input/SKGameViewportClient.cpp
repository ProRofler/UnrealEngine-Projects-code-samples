// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Core/Input/SKGameViewportClient.h"

#include "Kismet/GameplayStatics.h"
#include "SignificanceManager.h"

void USKGameViewportClient::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (World)
    {
        if (USignificanceManager *SignificanceManager = USignificanceManager::Get(World))
        {
            if (APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0))
            {
                TArray<FTransform> TransformArray;
                TransformArray.Add(PlayerPawn->GetTransform());

                SignificanceManager->Update(TArrayView<FTransform>(TransformArray));
            }
        }
    }
}
