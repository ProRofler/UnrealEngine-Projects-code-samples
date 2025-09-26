// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Interactables/SKCollectible.h"

#include "Core/Interface/SKInterfaceWeapon.h"

#include "SKWeapon.generated.h"

// TODO: a struct with weapon characteristics?

UCLASS()
class SIRKNIGHT_API ASKWeapon : public ASKCollectible, public ISKInterfaceWeapon
{
    GENERATED_BODY()

  public:
    ASKWeapon();
};
