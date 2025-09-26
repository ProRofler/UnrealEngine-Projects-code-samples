// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/S_TempHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

void AS_TempHUD::DrawHUD()
{
    Super::DrawHUD();

    //DrawCrosshair();
}

void AS_TempHUD::DrawCrosshair()
{
    TInterval<float> ScreenCenter(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
    // Draw horizontal line
    DrawLine(ScreenCenter.Min - 25-55, ScreenCenter.Max, ScreenCenter.Min + 25-55, ScreenCenter.Max, FLinearColor::Blue,
             2.0f);
    // Draw horizontal line
    DrawLine(ScreenCenter.Min-55, ScreenCenter.Max - 25, ScreenCenter.Min-55, ScreenCenter.Max + 25, FLinearColor::Blue,
             2.0f);
}

void AS_TempHUD::BeginPlay()
{
    Super::BeginPlay();
    DrawHud();

}

void AS_TempHUD::DrawHud()
{

    auto PlayerHudWidget = CreateWidget(GetWorld(), PlayerWidgetClass);
    if(!PlayerHudWidget) return;

    PlayerHudWidget->AddToViewport();
}
