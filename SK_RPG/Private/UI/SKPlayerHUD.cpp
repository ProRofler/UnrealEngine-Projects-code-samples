// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.


#include "UI/SKPlayerHUD.h"
#include "Engine/Canvas.h"

void ASKPlayerHUD::BeginPlay()
{
    Super::BeginPlay();
}


void ASKPlayerHUD::DrawHUD()
{
    Super::DrawHUD();
    DrawCrosshair();
}


void ASKPlayerHUD::DrawCrosshair()
{
    TInterval<float> ScreenCenter(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
    // Draw horizontal line
    DrawLine(ScreenCenter.Min - 10, ScreenCenter.Max, ScreenCenter.Min + 10, ScreenCenter.Max, FLinearColor::Blue,
             2.0f);
    // Draw horizontal line
    DrawLine(ScreenCenter.Min, ScreenCenter.Max - 10, ScreenCenter.Min, ScreenCenter.Max + 10, FLinearColor::Blue,
             2.0f);
}


