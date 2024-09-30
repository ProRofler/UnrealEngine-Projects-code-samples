#pragma once

#include "SKCoreTypes.generated.h"

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