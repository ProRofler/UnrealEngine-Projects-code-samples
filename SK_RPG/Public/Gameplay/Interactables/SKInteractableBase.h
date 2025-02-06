// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Core/Interface/SKInterfaceInteractable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SKInteractableBase.generated.h"

class UCapsuleComponent;

UCLASS(Blueprintable)
class SIRKNIGHT_API ASKInteractableBase : public AActor, public ISKInterfaceInteractable
{
    GENERATED_BODY()

  public:
    ASKInteractableBase();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SK Object settings")
    TObjectPtr<UStaticMeshComponent> BaseMesh;

    virtual void OnInteraction_Implementation(const AActor *TriggeredActor) override;

    // getters
    UFUNCTION(BlueprintCallable)
    FName GetInGameName() const { return InGameName; }

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Interactables")
    FName InGameName = TEXT("_DEFAULT_NAME_");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Interactables")
    bool bIsBreakable = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Interactables",
              meta = (EditCondition = "bIsBreakable"))
    bool bIsDestroyable = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SK Base properties | Interactables",
              meta = (EditCondition = "bIsBreakable", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float Integrity = 1.0f;

    UPROPERTY(EditAnywhere, Category = "SK Debugging")
    bool bEnableLogging = true;
};
