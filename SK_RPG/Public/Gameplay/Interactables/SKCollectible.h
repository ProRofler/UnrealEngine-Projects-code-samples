// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Core/Interface/SKInterfaceCollectible.h"
#include "Core/SKCoreTypes.h"

#include "CoreMinimal.h"
#include "Gameplay/Interactables/SKInteractableBase.h"
#include "SignificanceManager.h"

#include "SKCollectible.generated.h"

// Enum for significance presets, High means high priority, maximum settings, low means insignificance
enum class ESignificancePreset
{
    Low = 3,
    Medium = 2,
    High = 1
};

UCLASS()
class SIRKNIGHT_API ASKCollectible : public ASKInteractableBase, public ISKInterfaceCollectible
{
    GENERATED_BODY()

  public:
    ASKCollectible();

    UFUNCTION(BlueprintCallable)
    const FORCEINLINE int32 &GetItemQuantity() const { return Quantity; }

    UFUNCTION(BlueprintCallable)
    const FORCEINLINE ECollectibleType GetCollectibleType() const { return CollectibleType; }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE void SetCollectibleType(const ECollectibleType _CollectibleType) { CollectibleType = _CollectibleType; }

    virtual void OnInteraction_Implementation(const AActor *TriggeredActor) override;

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Collectibles")
    int32 Quantity = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Collectibles",
              meta = (ClampMin = "0.0001", UIMin = "0.0"))
    float Weight = 1.0f;

    // Significance Manager implementation
    float SignficanceFunction(USignificanceManager::FManagedObjectInfo *ObjectInfo, const FTransform &Viewpoint);
    void PostSignficanceFunction(USignificanceManager::FManagedObjectInfo *ObjectInfo, float OldSignificance,
                                 float Significance, bool bFinal);

  private:
    void SetSignificanceMode(const ESignificancePreset Preset);

    ECollectibleType CollectibleType = ECollectibleType::Misc;

    static int32 CollectiblesAmount;

    float InitialLinearDampening;
    float InitialAngularDampening;
};
