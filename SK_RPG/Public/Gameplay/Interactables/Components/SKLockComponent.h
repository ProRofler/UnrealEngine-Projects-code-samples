// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SKLockComponent.generated.h"

class ASKKeyItem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIRKNIGHT_API USKLockComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USKLockComponent();

    UFUNCTION()
    const FORCEINLINE bool IsLocked() const { return bIsLocked; }
    const bool TryUnlocking(const AActor *UnlockInitiator);

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "SK Lock Settings")
    bool bIsLocked = true;

    UPROPERTY(EditInstanceOnly, Category = "SK Lock Settings")
    TSubclassOf<ASKKeyItem> KeyClass;

  private:
    void Unlock(const AActor *UnlockInitiator) { bIsLocked = false; };
};
