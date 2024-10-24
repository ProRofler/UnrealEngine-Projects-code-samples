// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "SKCoreTypes.generated.h"

class ASKCollectible;

UENUM()
enum class EMovementType
{
    EIdle,
    EWalking,
    ERunning,
    ESprinting,
    ECrouching
};

UENUM()
enum class EActionType
{
    ENone,
    EAttacking,
    EBlocking,
    EGrabbing,
    ERotating
};

struct FCharacterAttributes
{
    // skills
    float Athletics = 65.0f;
};

USTRUCT(BlueprintType)
struct FInteractableProperties
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
    FName InGameName = TEXT("_DEFAULT_NAME_");
};

USTRUCT(BlueprintType)
struct FInventoryItemData
{
    GENERATED_USTRUCT_BODY()
    FName Name = TEXT("Default name");
    TObjectPtr<ASKCollectible> Item = nullptr;
};