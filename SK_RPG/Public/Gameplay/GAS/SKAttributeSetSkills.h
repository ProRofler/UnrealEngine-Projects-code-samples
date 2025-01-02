// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreMinimal.h"
#include "SKAttributeSetSkills.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)                                                                   \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                         \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                       \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                       \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SIRKNIGHT_API USKAttributeSetSkills : public UAttributeSet
{
    GENERATED_BODY()

  public:
    USKAttributeSetSkills(){};

    ATTRIBUTE_ACCESSORS(USKAttributeSetSkills, Athletics);

  protected:
    UPROPERTY(BlueprintReadOnly, Category = "Skills")
    FGameplayAttributeData Athletics;
};
