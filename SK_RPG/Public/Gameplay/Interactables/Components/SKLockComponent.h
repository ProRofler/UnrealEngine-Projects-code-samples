// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SKLockComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIRKNIGHT_API USKLockComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USKLockComponent();

    FORCEINLINE const bool GetIsLocked() const { return bIsLocked; }
    const bool TryUnlocking(const AActor *UnlockInitiator);

    FORCEINLINE const FName &GetKeyID() const { return KeyID; }

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "SK Lock Settings")
    bool bIsLocked = true;

    UPROPERTY(EditInstanceOnly, Category = "SK Lock Settings")
    FName KeyID = TEXT("None"); // For now interactable name == key ID

  private:
    void Unlock() { bIsLocked = false; };
};
