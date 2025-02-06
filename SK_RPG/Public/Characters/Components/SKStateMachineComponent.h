// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "SKStateMachineComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnActionRequestAddSignature, FGameplayTag &, AddedTag);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIRKNIGHT_API USKStateMachineComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USKStateMachineComponent();

    FGameplayTag &GetMovementState() { return MovementState; }
    FGameplayTag &GetActionState() { return ActionState; }
    void RequestAction(const FGameplayTag &ActionRequestTag);

  protected:
    virtual void BeginPlay() override;

  private:
    FGameplayTag MovementState;
    FGameplayTag ActionState;
    FGameplayTagContainer ActionRequest;
};
