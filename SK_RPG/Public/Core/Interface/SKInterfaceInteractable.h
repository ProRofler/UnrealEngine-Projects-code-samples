// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SKInterfaceInteractable.generated.h"

UINTERFACE(BlueprintType)
class USKInterfaceInteractable : public UInterface
{
    GENERATED_BODY()
};

class SIRKNIGHT_API ISKInterfaceInteractable
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Implemented interfaces")
    void OnInteraction(const AActor *TriggeredActor);
};
