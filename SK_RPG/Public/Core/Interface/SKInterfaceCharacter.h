// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SKInterfaceCharacter.generated.h"

UINTERFACE(MinimalAPI)
class USKInterfaceCharacter : public UInterface
{
    GENERATED_BODY()
};

class SIRKNIGHT_API ISKInterfaceCharacter
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    // UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Implemented interfaces")
    // void AddToInventory(const AActor* Item);
};
