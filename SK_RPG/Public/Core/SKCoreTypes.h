// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "SKCoreTypes.generated.h"

UENUM(BlueprintType)
enum class ECollectibleType : uint8
{
    Weapon UMETA(DisplayName = "Weapon"),
    Key UMETA(DisplayName = "Key"),
    Misc UMETA(DisplayName = "Misc")
};