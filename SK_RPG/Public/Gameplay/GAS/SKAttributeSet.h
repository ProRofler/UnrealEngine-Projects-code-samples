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

UCLASS()
class SIRKNIGHT_API USKAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

  public:
    USKAttributeSet();

    ATTRIBUTE_ACCESSORS(USKAttributeSet, Health);
    ATTRIBUTE_ACCESSORS(USKAttributeSet, Stamina);

  protected:
    UPROPERTY(BlueprintReadOnly, Category = "Main attributes")
    FGameplayAttributeData Health;

    UPROPERTY(BlueprintReadOnly, Category = "Main attributes")
    FGameplayAttributeData Stamina;
};
