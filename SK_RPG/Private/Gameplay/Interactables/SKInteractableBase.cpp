// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Gameplay/Interactables/SKInteractableBase.h"
#include "Characters/SKBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"

#include "Kismet/KismetMaterialLibrary.h"

#include "SKAssetManager.h"
#include "Utils/DataAssets/SKMaterialsPrimaryDataAsset.h"

#include "Core/Performace/SKSignificanceManagerComponent.h"

int32 ASKInteractableBase::InteractablesCount = 0;

ASKInteractableBase::ASKInteractableBase()
{
    PrimaryActorTick.bCanEverTick = false;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Object mesh");
    SetRootComponent(BaseMesh);

    BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BaseMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    InteractableID = FGuid::NewGuid();

    SigninficanceManagerComponent =
        CreateDefaultSubobject<USKSignificanceManagerComponent>("Significance manager component");
}

void ASKInteractableBase::BeginPlay()
{
    Super::BeginPlay();
    InteractablesCount++;
    SetupOverlayMaterial();

    if (BaseMesh && BaseMesh->IsSimulatingPhysics())
    {
        InitialLinearDampening = BaseMesh->BodyInstance.LinearDamping;
        InitialAngularDampening = BaseMesh->BodyInstance.AngularDamping;
    }
}

void ASKInteractableBase::EndPlay(const EEndPlayReason::Type Reason)
{
    Super::EndPlay(Reason);
    InteractablesCount--;
}

#if !UE_BUILD_SHIPPING
void ASKInteractableBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

#if WITH_EDITORONLY_DATA
    if (bDrawCenterSphere) DrawCenter();
#endif
}
#endif

void ASKInteractableBase::DrawCenter()
{
    FVector center, extend;
    GetActorBounds(false, center, extend, true);

    DrawDebugSphere(GetWorld(), center, 8.0f, 8, FColor::Green);
}

void ASKInteractableBase::SetupOverlayMaterial()
{
    FPrimaryAssetId AssetId = FPrimaryAssetId(TEXT("SKMaterialsPrimaryDataAsset"), TEXT("PDA_SKCoreMaterials"));
    if (!AssetId.IsValid()) return;

    USKAssetManager &AssetManager = USKAssetManager::Get();

    AssetManager.LoadPrimaryAsset(
        AssetId, {}, FStreamableDelegate::CreateLambda([this, AssetId]() {
            UObject *Loaded = UAssetManager::Get().GetPrimaryAssetObject(AssetId);
            if (!Loaded)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to load primary asset: %s"), *AssetId.ToString());
                return;
            }

            USKMaterialsPrimaryDataAsset *DataAsset = Cast<USKMaterialsPrimaryDataAsset>(Loaded);
            if (!DataAsset)
            {
                UE_LOG(LogTemp, Error, TEXT("Asset %s is not a valid USKMaterialsPrimaryDataAsset"),
                       *AssetId.ToString());
                return;
            }

            UMaterialInterface *Material = DataAsset->OutlineMaterial.IsValid()
                                               ? DataAsset->OutlineMaterial.Get()
                                               : DataAsset->OutlineMaterial.LoadSynchronous();

            if (!Material)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to load outline material from asset: %s"), *AssetId.ToString());
                return;
            }

            OverlayDynMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(
                this, Material, *GetName().Append("_OverlayMaterialInstance"));

            BaseMesh->SetOverlayMaterial(OverlayDynMat);

            if (bEnableLogging)
            {
                UE_LOG(LogTemp, Display, TEXT("Overlay material set: %s"), *Material->GetName());
            }
        }));
}

void ASKInteractableBase::OnInteraction_Implementation(const AActor *TriggeredActor)
{
    UE_LOGFMT(
        LogSKInteractions, Warning,
        "Attempt to call an empty method of a base class in {1}, is this intended? This method should not be called",
        ("1", GetName()));
}

void ASKInteractableBase::SetIsFocused_Implementation(const bool Value)
{
    if (OverlayDynMat)
    {
        float ConvertedValue = Value ? 1.f : 0.f;
        OverlayDynMat->SetScalarParameterValue("IsFocused", ConvertedValue);
    }
}

void ASKInteractableBase::SetIsInReach_Implementation(const bool Value)
{
    if (OverlayDynMat)
    {
        float ConvertedValue = Value ? 1.f : 0.f;
        OverlayDynMat->SetScalarParameterValue("IsActive", ConvertedValue);
    }
}
