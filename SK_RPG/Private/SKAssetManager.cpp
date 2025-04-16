// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "SKAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "Gameplay/GAS/SKNativeGameplayTags.h"

USKAssetManager &USKAssetManager::Get()
{
    check(GEngine);

    USKAssetManager *SKAssetManager = Cast<USKAssetManager>(GEngine->AssetManager);
    return *SKAssetManager;
}

void USKAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();

    FSKGameplayTags::InitializeNativeGameplayTags();

    // This is required to use Target Data!
    UAbilitySystemGlobals::Get().InitGlobalData();
}
