// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SKEquippableBase.generated.h"

UCLASS(NotBlueprintable)
class SIRKNIGHT_API ASKEquippableBase : public AActor
{
    GENERATED_BODY()

  public:
    ASKEquippableBase();
    FORCEINLINE UStaticMesh *GetStaticMesh() const { return BaseMesh->GetStaticMesh(); }
    FORCEINLINE UStaticMeshComponent *GetStaticMeshComponent() const { return BaseMesh; }

  protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "SK Weapon Settings")
    UStaticMeshComponent *BaseMesh;
};
