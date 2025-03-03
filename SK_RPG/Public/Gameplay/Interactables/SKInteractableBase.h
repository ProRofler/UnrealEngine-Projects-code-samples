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

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "SK Object settings")
    TObjectPtr<UStaticMeshComponent> BaseMesh;

    virtual void OnInteraction_Implementation(const AActor *TriggeredActor) override;

#if !UE_BUILD_SHIPPING
    virtual void Tick(float DeltaSeconds);
#endif

#if WITH_EDITORONLY_DATA
    UPROPERTY(EditAnywhere, Category = "SK Debugging|Debug drawing ")
    bool bDrawCenterSphere = false;
#endif

    // getters
    UFUNCTION(BlueprintCallable)
    FORCEINLINE FName GetInteractableName() const { return InteractableName; }

    UFUNCTION(BlueprintCallable)
    const FORCEINLINE FGuid GetInteractableID() const { return InteractableID; }

    //UFUNCTION(BlueprintCallable)
    //void SetInteractableName(const FName &ItemName) { InteractableName = ItemName; };

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SK Base properties | Interactables")
    FGuid InteractableID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Interactables")
    FName InteractableName = TEXT("_DEFAULT_NAME_");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Interactables")
    bool bIsBreakable = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SK Base properties | Interactables",
              meta = (EditCondition = "bIsBreakable"))
    bool bIsDestroyable = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SK Base properties | Interactables",
              meta = (EditCondition = "bIsBreakable", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float Integrity = 1.0f;

    UPROPERTY(EditAnywhere, Category = "SK Debugging")
    bool bEnableLogging = true;

    void DrawCenter();

};
