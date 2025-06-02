// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SKInterfaceCharacter.generated.h"

class USKInventoryComponent;
class USKWeaponComponent;
class USKInteractionComponent;

UINTERFACE(BlueprintType)
class USKInterfaceCharacter : public UInterface
{
    GENERATED_BODY()
};

class SIRKNIGHT_API ISKInterfaceCharacter
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SK Character interface")
    USKInventoryComponent *GetInventoryComponent() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SK Character interface")
    USKWeaponComponent *GetWeaponComponent() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SK Character interface")
    USKInteractionComponent *GetInteractionComponent() const;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SK Character interface")
    void ToggleCombatPhysics();
};
