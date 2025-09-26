// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MyEnvQueryTest_AmmoTakeCheck.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/S_PickupBase.h"

UMyEnvQueryTest_AmmoTakeCheck::UMyEnvQueryTest_AmmoTakeCheck(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UMyEnvQueryTest_AmmoTakeCheck::RunTest(FEnvQueryInstance &QueryInstance) const
{

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const auto DataOwner = QueryInstance.Owner.Get();
        BoolValue.BindData(DataOwner, QueryInstance.QueryID);

        bool bDesiredValue = BoolValue.GetValue();

        const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const auto PickupActor = Cast<AS_PickupBase>(ItemActor);
        if (!PickupActor)
            continue;

        const auto CouldBeTaken = PickupActor->CouldBeTaken();
        if (CouldBeTaken)

            It.SetScore(TestPurpose, FilterType, CouldBeTaken, bDesiredValue);
    }
}
