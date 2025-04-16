// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreMinimal.h"
#include "SKAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)                                                                   \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                         \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                       \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                       \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FSKAttributeChangeData
{
    // attribute related
    float ChangedAmount;
    float CurrentValue;
    float MaxValue;

    FGameplayTag AttributeTag;

    // Tags for gameplay events
    FGameplayTag EventIncreasedTag;
    FGameplayTag EventDecreasedTag;
    FGameplayTag EventDepletedTag;
    FGameplayTag EventFullTag;

    GENERATED_BODY()
};

UCLASS()
class SIRKNIGHT_API USKAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

  public:
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

    ATTRIBUTE_ACCESSORS(USKAttributeSet, Health);
    ATTRIBUTE_ACCESSORS(USKAttributeSet, MaxHealth);
    ATTRIBUTE_ACCESSORS(USKAttributeSet, Stamina);
    ATTRIBUTE_ACCESSORS(USKAttributeSet, MaxStamina);

  protected:
    UPROPERTY(BlueprintReadOnly, Category = "Main attributes")
    FGameplayAttributeData Health;

    UPROPERTY(BlueprintReadOnly, Category = "Main attributes")
    FGameplayAttributeData MaxHealth;

    UPROPERTY(BlueprintReadOnly, Category = "Main attributes")
    FGameplayAttributeData Stamina;

    UPROPERTY(BlueprintReadOnly, Category = "Main attributes")
    FGameplayAttributeData MaxStamina;
};
