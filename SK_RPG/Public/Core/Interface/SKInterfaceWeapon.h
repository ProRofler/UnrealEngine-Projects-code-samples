// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Gameplay/Equipables/SKEquippableBase.h"

#include "SKInterfaceWeapon.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class USKInterfaceWeapon : public UInterface
{
    GENERATED_BODY()
};

class SIRKNIGHT_API ISKInterfaceWeapon
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SK Weapon Interface")
    TSubclassOf<ASKEquippableBase> GetEquippableWeaponClass() const;
};
