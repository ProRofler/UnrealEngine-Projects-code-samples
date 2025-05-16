// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"

#include "SKGameViewportClient.generated.h"

UCLASS()
class SIRKNIGHT_API USKGameViewportClient : public UGameViewportClient
{
    GENERATED_BODY()

    public:
    virtual void Tick(float DeltaTime) override;
};
