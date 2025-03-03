// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SKInterfaceCharacter.generated.h"

class USKInventoryComponent;

UINTERFACE(MinimalAPI)
class USKInterfaceCharacter : public UInterface
{
    GENERATED_BODY()
};

class SIRKNIGHT_API ISKInterfaceCharacter
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character interface")
    USKInventoryComponent *GetInventoryComponent() const;
};
