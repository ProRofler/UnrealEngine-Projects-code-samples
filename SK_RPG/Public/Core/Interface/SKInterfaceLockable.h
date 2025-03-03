// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SKInterfaceLockable.generated.h"

class USKLockComponent;

UINTERFACE(MinimalAPI)
class USKInterfaceLockable : public UInterface
{
    GENERATED_BODY()
};

class SIRKNIGHT_API ISKInterfaceLockable
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lock interface")
    const USKLockComponent *GetLockComponent() const;
};
