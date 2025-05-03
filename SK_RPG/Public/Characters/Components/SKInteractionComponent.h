// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Characters/Components/SKCharacterComponentBase.h"
#include "CoreMinimal.h"
#include "SKInteractionComponent.generated.h"

class UCapsuleComponent;

UCLASS()
class SIRKNIGHT_API USKInteractionComponent : public USKCharacterComponentBase
{
    GENERATED_BODY()

  public:
    USKInteractionComponent();

  protected:
    virtual void BeginPlay() override;

  public:
    UFUNCTION(BlueprintPure, Category = "SK Character|Interactions")
    AActor *GetInteractionTarget() const { return InteractionTarget.Get(); }

    UFUNCTION(BlueprintPure, Category = "SK Character|Interactions")
    UCapsuleComponent *GetInteractionZone() const { return InteractionZone.Get(); }

    UFUNCTION(BlueprintCallable, Category = "SK Character|Interactions")
    virtual void Interact();

  public:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SK Interactions settings")
    float GrabDistance = 150.0f;

  protected:
    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UCapsuleComponent> InteractionZone;

    TSet<AActor *> InteractablesInVicinity;
    TWeakObjectPtr<AActor> InteractionTarget;

  private:
    void HandleInteractionsTimer();
    virtual void HandleInteractionActor();
    FHitResult TraceToBoundingBox(const AActor *OtherActor) const;
    AActor *GetLookedAtActor() const;

  private:
    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                      int32 OtherBodyIndex);

    /************************************ MULTITHREADING  ******************************************/
  protected:
    mutable FRWLock DataGuard;

    void AsyncInteractionHandle();

    /************************************ Timers ******************************************/
  private:
    FTimerHandle InteractableActiveUpdateTimer;
};
