// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Utils/Libraries/SKFunctionLibrary.h"

#include "Characters/SKBaseCharacter.h"

FName USKFunctionLibrary::GetMontageSectionNameByIndex(const UAnimMontage *Montage, const int32 Index)
{
    if (Montage)
    {
        return Montage->GetSectionName(Index);
    }

    return FName("None");
}
