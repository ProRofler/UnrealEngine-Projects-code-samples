// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Core/Interface/SKInterfaceInteractable.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SKInteractableBase.generated.h"

class UCapsuleComponent;
class USKSignificanceManagerComponent;

UCLASS(Blueprintable)
class SIRKNIGHT_API ASKInteractableBase : public AActor, public ISKInterfaceInteractable
{
    GENERATED_BODY()

  public:
    ASKInteractableBase();

    virtual void OnInteraction_Implementation(const AActor *TriggeredActor) override;
    virtual void SetIsFocused_Implementation(const bool Value) override;
    virtual void SetIsInReach_Implementation(const bool Value) override;

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

    FORCEINLINE static int32 GetInteractablesCount() { return InteractablesCount; }
    FORCEINLINE UStaticMeshComponent *GetMesh() { return BaseMesh; }

    // Is this needed?
    float InitialLinearDampening;
    float InitialAngularDampening;

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;

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

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "SK Object settings",
              meta = (EditCondition = "bCanEditMesh"))
    TObjectPtr<UStaticMeshComponent> BaseMesh;

    bool bCanEditMesh = true; // For disabling direct mesh editing if needed

    // components
    TObjectPtr<USKSignificanceManagerComponent> SigninficanceManagerComponent;

  private:
    void SetupOverlayMaterial();

    UMaterialInstanceDynamic *OverlayDynMat = nullptr;

    static int32 InteractablesCount;
};
