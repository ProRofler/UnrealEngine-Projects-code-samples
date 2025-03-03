// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "SKCharacterComponentBase.generated.h"

class ASKBaseCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIRKNIGHT_API USKCharacterComponentBase : public UActorComponent
{
    GENERATED_BODY()

  public:
    USKCharacterComponentBase();

    UFUNCTION(BlueprintPure, Category = "SK Character")
    FORCEINLINE ASKBaseCharacter *GetSKOwnerCharacter() const { return SKCharacter.Get(); }

  protected:
    virtual void BeginPlay() override;

  private:
    TWeakObjectPtr<ASKBaseCharacter> SKCharacter = nullptr;

    void InitializeSKCharacterOwner();
};
