// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SignificanceManager.h"

//
#include "SKSignificanceManagerComponent.generated.h"

enum class ESignificancePreset
{
    Low = 3,
    Medium = 2,
    High = 1
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIRKNIGHT_API USKSignificanceManagerComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USKSignificanceManagerComponent();

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;

    float SignficanceFunction(USignificanceManager::FManagedObjectInfo *ObjectInfo, const FTransform &Viewpoint);
    void PostSignficanceFunction(USignificanceManager::FManagedObjectInfo *ObjectInfo, float OldSignificance,
                                 float Significance, bool bFinal);

  private:
    void SetSignificanceMode(const ESignificancePreset Preset);
};
