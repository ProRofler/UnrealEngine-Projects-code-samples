// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Interactables/SKInteractableBase.h"

#include "SKActivator.generated.h"

class ASKDoorway;

/*NOTE: Interface method OnInteract for this class must be implemented in BP. This is due to possible variety of
activators*/

UCLASS()
class SIRKNIGHT_API ASKActivator : public ASKInteractableBase
{
    GENERATED_BODY()

  protected:
    UPROPERTY(EditAnywhere, Category = "SK Activator Settings")
    bool bRandomDelayActivation = true;

    UPROPERTY(EditAnywhere, Category = "SK Activator Settings",
              meta = (EditCondition = "bRandomDelayActivation", ClampMin = "0.05", ClampMax = "100.0", UIMin = "0.05",
                      UIMax = "2.0"))
    float RandomMin = 0.2f;

    UPROPERTY(EditAnywhere, Category = "SK Activator Settings",
              meta = (EditCondition = "bRandomDelayActivation", ClampMin = "0.05", ClampMax = "100.0", UIMin = "0.05",
                      UIMax = "2.0"))
    float RandomMaxVariation = 1.0f;

    UPROPERTY(EditAnywhere, Category = "SK Activator Settings",
              meta = (EditCondition = "!bRandomDelayActivation", ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0",
                      UIMax = "100.0"))
    float DelayTime = 0.5f;

    UPROPERTY(EditInstanceOnly, Category = "SK Activator settings")
    TArray<TWeakObjectPtr<ASKDoorway>> ActorsToActivate;

    /*Call this function in BP to startup a logic of objects in container above*/
    UFUNCTION(BlueprintCallable, Category = "SK Interactions")
    void ActivateAll();

    UFUNCTION(BlueprintCallable, Category = "SK Interactions")
    bool IsAnyDoorActive() const;
};
