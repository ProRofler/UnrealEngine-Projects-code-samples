// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "UI/Widgets/SKUserWidgetBase.h"

void USKUserWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    PlayerHUD = GetSKPlayerHud();
    check(PlayerHUD.IsValid());
}

ASKPlayerHUD *USKUserWidgetBase::GetSKPlayerHud()
{
    if (const auto PC = GetOwningPlayer())
    {
        const auto HUD = Cast<ASKPlayerHUD>(PC->GetHUD());

        return HUD ? HUD : nullptr;
    }

    return nullptr;
}
