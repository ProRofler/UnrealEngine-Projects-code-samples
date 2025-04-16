#pragma once

#include "EnhancedInputData.generated.h"

class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FEnhancedInputData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mapping context")
    UInputMappingContext *InputMapping = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction *MovingAction = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
    UInputAction *LookAction = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
    UInputAction *JumpAction = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
    UInputAction *WalkAction = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interactions")
    UInputAction *AltAction = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interactions")
    UInputAction *InteractionActionHold = nullptr;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    UInputAction *InventoryToggleAction = nullptr;
};