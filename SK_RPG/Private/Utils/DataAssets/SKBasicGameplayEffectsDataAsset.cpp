// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Utils/DataAssets/SKBasicGameplayEffectsDataAsset.h"
#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"


void USKBasicGameplayEffectsDataAsset::ValidateData() const //Check every field if it's valid and crash if not
{
     for (TFieldIterator<FProperty> It(GetClass()); It; ++It)
    {
        const void *ValuePtr = It->ContainerPtrToValuePtr<void>(this);

        if (FObjectProperty *ObjectProperty = CastField<FObjectProperty>(*It))
        {
            UObject *Obj = ObjectProperty->GetObjectPropertyValue(ValuePtr);
            if (!Obj)
            {
                UE_LOG(LogTemp, Error, TEXT("Field %s is NULL in %s"), *It->GetName(), *GetName());
                checkNoEntry();
            }
        }
        else if (FStructProperty *StructProperty = CastField<FStructProperty>(*It))
        {
            if (StructProperty->Struct->GetStructureSize() == 0)
            {
                UE_LOG(LogTemp, Warning, TEXT("Struct %s in %s has zero size"), *It->GetName(), *GetName());
                checkNoEntry();
            }
        }
    } 
}
