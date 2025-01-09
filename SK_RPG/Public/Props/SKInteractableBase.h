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
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Object settings")
    TObjectPtr<UStaticMeshComponent> BaseMesh;

    virtual void OnInteraction_Implementation(const AActor *TriggeredActor) override;

    // getters
    UFUNCTION(BlueprintCallable)
    FName GetInGameName() const { return InGameName; }

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base properties | Interactables")
    FName InGameName = TEXT("_DEFAULT_NAME_");
};
