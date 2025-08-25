// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Characters/Components/SKCharacterComponentBase.h"
#include "CoreMinimal.h"

#include <atomic>

#include "SKInteractionComponent.generated.h"

class UCapsuleComponent;

DECLARE_DELEGATE(FOnVicinityChangedSignature);

UCLASS()
class SIRKNIGHT_API USKInteractionComponent : public USKCharacterComponentBase
{
    GENERATED_BODY()

  public:
    USKInteractionComponent();

  protected:
    virtual void BeginPlay() override;

  public:
    UFUNCTION(BlueprintPure, Category = "SK Character|Interaction component")
    AActor *GetInteractionTarget() const { return InteractionTarget.Get(); }

    UFUNCTION(BlueprintPure, Category = "SK Character|Interaction component")
    UCapsuleComponent *GetInteractionZone() const { return InteractionZone.Get(); }

    UFUNCTION(BlueprintCallable, Category = "SK Character|Interaction component")
    virtual void Interact();

  public:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SK Interaction component settings")
    float GrabDistance = 150.0f;

    UFUNCTION(BlueprintPure, Category = "SK Interaction component")
    AActor *GetLookedAtActor(const TArray<AActor *> &Actors, const double Threshold) const;

  private:
    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                      int32 OtherBodyIndex);

    /************************************ DATA  ******************************************/
  protected:
    void HandleVicinityChanged();

    FOnVicinityChangedSignature OnVicinityChanged;

  private:
    TSet<AActor *> InteractablesInVicinity;
    TWeakObjectPtr<AActor> InteractionTarget;

    UPROPERTY()
    TObjectPtr<UCapsuleComponent> InteractionZone;

    /************************************ MULTITHREADING  ******************************************/
  private:
    mutable FRWLock DataGuard;
    std::atomic<bool> bIsActiveAtomic{false};

    void AsyncInteractionHandle();
};
