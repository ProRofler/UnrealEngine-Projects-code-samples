// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.

#include "Characters/Components/SKInventoryComponent.h"
#include "Characters/SKBaseCharacter.h"
#include "Core/Interface/SKInterfaceInteractable.h"
#include "Core/SKLogCategories.h"
#include "Logging/StructuredLog.h"
#include "Props/SKCollectible.h"

/********************* DEFAULT *********************/
USKInventoryComponent::USKInventoryComponent() { PrimaryComponentTick.bCanEverTick = false; }

void USKInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    OwningCharacter = GetOwningCharacter();
    // InitDelegates();
}

/********************* Items handling *********************/
void USKInventoryComponent::AddToInventory(AActor *PickedItem)
{
    // TODO: stacking logic

    if (!PickedItem) return;

    InventoryData.Add(PickedItem);
    ISKInterfaceInteractable::Execute_OnInteraction(PickedItem, OwningCharacter);
    SortInventory();

    if (OnInventoryUpdated.IsBound())
    {
        OnInventoryUpdated.Execute();
    }
    else
    {
        UE_LOG(LogSKInteractions, Error, TEXT("Item pickup delegate has failed!"));
        return;
    }

    /** LOG **/
    UE_LOGFMT(LogSKInteractions, Display, "Actor: {0} picked up item: {1}", this->GetName(), PickedItem->GetName());
}

void USKInventoryComponent::RemoveFromInventory(AActor *ItemToRemove)
{
    if (!ItemToRemove) return;

    if (OnItemPickup.IsBound())
    {
        OnItemPickup.RemoveAll(ItemToRemove);
    }

    if (ItemToRemove->Implements<USKInterfaceInteractable>())
    {
        ISKInterfaceInteractable::Execute_OnInteraction(ItemToRemove, OwningCharacter);
    }
    else
    {
        checkNoEntry();
    }

    InventoryData.Remove(ItemToRemove);

    OnInventoryUpdated.ExecuteIfBound();

    // LOG
    UE_LOGFMT(LogSKInteractions, Display, "Actor: {0} Dropped item: {1}", this->GetName(), ItemToRemove->GetName());
}

void USKInventoryComponent::SortInventory()
{

    InventoryData.Sort([](const TWeakObjectPtr<AActor> &A, const TWeakObjectPtr<AActor> &B) {
        auto Acast = Cast<ASKCollectible>(A.Get());
        auto Bcast = Cast<ASKCollectible>(B.Get());

        if (Acast && Bcast)
        {
            return Acast->GetInGameName().LexicalLess(Bcast->GetInGameName());
        }
        else
        {
            return Acast != nullptr;
        }
    });
}

/********************* Initialization *********************/
ASKBaseCharacter *USKInventoryComponent::GetOwningCharacter()
{
    auto owner = GetOwner();
    auto castToChar = Cast<ASKBaseCharacter>(owner);
    ensureMsgf(castToChar, TEXT("No owner found!"));
    return castToChar;
}

void USKInventoryComponent::InitDelegates()
{
    // OwningCharacter->OnItemPickup.AddDynamic(this, &USKInventoryComponent::AddToInventory);
}
