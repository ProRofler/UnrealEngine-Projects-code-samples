// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SKEquippableBase.generated.h"

UCLASS()
class SIRKNIGHT_API ASKEquippableBase : public AActor
{
    GENERATED_BODY()

  public:
    ASKEquippableBase();
    UStaticMesh *GetStaticMesh() const;

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "SK Weapon Settings")
    UStaticMeshComponent *BaseMesh;
};
