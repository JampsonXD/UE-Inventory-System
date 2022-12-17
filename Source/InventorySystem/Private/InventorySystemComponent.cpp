// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystemComponent.h"

AActor* UInventorySystemComponent::GetOwningActor() const
{
	return OwningActor;
}

AActor* UInventorySystemComponent::GetAvatarActor() const
{
	return AvatarActor;
}

bool UInventorySystemComponent::AddItem(UItem* Item, int StackCount, bool bAutoEquip)
{
	if(!Item || StackCount <= 0)
	{
		return false;
	}

	FInventorySlotData OldSlot;
	GetInventorySlotForItem(Item, OldSlot);

	FInventorySlotData NewSlot = OldSlot;
	NewSlot.UpdateSlotData(FInventorySlotData(StackCount), Item->GetMaxStackCount());

	/* If our data changed after trying to update */
	if(NewSlot != OldSlot)
	{
		InventoryMap.Add(Item, NewSlot);

		/* If we have a delegate listening for this items stack count to change, broadcast to it the old and new stack values */
		if(ItemStackCountChangedMap.Find(Item))
		{
			ItemStackCountChangedMap.FindChecked(Item).Broadcast(OldSlot.StackCount, NewSlot.StackCount);
		}

		OnItemChanged.Broadcast(Item, OldSlot.StackCount == 0 ? EInventorySlotChangeType::Added : EInventorySlotChangeType::StackChange);

		if(bAutoEquip)
		{
			TryEquipItem(Item);
		}

		return true;
	}

	return false;
}

bool UInventorySystemComponent::RemoveItem(UItem* Item, int StackCount)
{
	if(!Item)
	{
		return false;
	}

	FInventorySlotData OldSlot;
	GetInventorySlotForItem(Item, OldSlot);

	if(!OldSlot.IsValid())
	{
		return false;
	}

	FInventorySlotData NewSlot = OldSlot;

	NewSlot.StackCount = StackCount <= 0 ? 0 : NewSlot.StackCount - StackCount;

	if(NewSlot.StackCount > 0)
	{
		InventoryMap.Add(Item, NewSlot);
		OnItemChanged.Broadcast(Item, EInventorySlotChangeType::StackChange);

		if(ItemStackCountChangedMap.Find(Item))
		{
			ItemStackCountChangedMap[Item].Broadcast(OldSlot.StackCount, NewSlot.StackCount);
		}
	}
	else
	{
		InventoryMap.Remove(Item);
		OnItemChanged.Broadcast(Item, EInventorySlotChangeType::Removed);
	}

	return true;
}

bool UInventorySystemComponent::SetItemStateData(UItem* Item, FItemStateData ItemStateData)
{
	FInventorySlotData Slot;
	GetInventorySlotForItem(Item, Slot);

	if(Slot.IsValid())
	{
		Slot.ItemData = ItemStateData;
		InventoryMap.Add(Item, Slot);
		return true;
	}

	return false;
}

FItemStateData UInventorySystemComponent::GetItemStateData(UItem* Item)
{
	FInventorySlotData Slot;
	GetInventorySlotForItem(Item, Slot);
	return Slot.ItemData;
}

bool UInventorySystemComponent::GetInventoryItems(FPrimaryAssetType ItemType, TArray<UItem*>& OutItems)
{
	if(!ItemType.IsValid())
	{
		InventoryMap.GetKeys(OutItems);
		return !OutItems.IsEmpty();
	}

	for(TPair<UItem*, FInventorySlotData>& Pair : InventoryMap)
	{
		if(Pair.Key && Pair.Key->GetItemType() == ItemType)
		{
			OutItems.Add(Pair.Key);
		}
	}

	return !OutItems.IsEmpty();
}

int UInventorySystemComponent::GetItemStackCount(const UItem* Item)
{
	if(!Item)
	{
		return 0;
	}

	if(const FInventorySlotData* SlotData = InventoryMap.Find(Item))
	{
		return SlotData->StackCount;
	}

	return 0;
}

void UInventorySystemComponent::InitInventorySystemComponent()
{
	// Remove any items before adding our defaults
	for(TPair<UItem*, FInventorySlotData>& Pair : InventoryMap)
	{
		if(Pair.Key)
		{
			RemoveItem(Pair.Key, -1);
		}
	}

	if(!DefaultEquipmentSlots.IsEmpty())
	{
		EquipmentMap.Reserve(DefaultEquipmentSlots.Num());

		/* Loop through our map of slot types to slot amounts
		 * Add a new equipment slot for each slot up the total amount
		 */
		for(const TPair<FPrimaryAssetType, int32>& Pair : DefaultEquipmentSlots)
		{
			for(int i = 0; i < Pair.Value; i++)
			{
				EquipmentMap.Add(FEquippedSlot(Pair.Key, i));
			}
		}
	}

	// Add Default Inventory Items to our Inventory
	for(const FDefaultInventoryData& InventorySlot : DefaultInventoryItemData)
	{
		AddItem(InventorySlot.Item, InventorySlot.StackCount, InventorySlot.bEquipOnAdded);
	}
}

bool UInventorySystemComponent::HasItem(const UItem* Item)
{
	/* Return false if we pass in no item */
	if (!Item) return false;

	/* Check if any of our inventory slots contains a reference to the data asset */
	const FInventorySlotData* Data = InventoryMap.Find(Item);
	return Data ? true : false;
}

FOnItemChanged& UInventorySystemComponent::GetOnItemChangedDelegate()
{
	return OnItemChanged;
}

FOnItemStackCountChanged& UInventorySystemComponent::RegisterItemStackCountChangedEvent(const UItem* Item)
{
	return ItemStackCountChangedMap.FindOrAdd(Item);
}

void UInventorySystemComponent::InitActorInfo(AActor* InOwningActor, AActor* InAvatarActor)
{
	OwningActor = InOwningActor;
	AvatarActor = InAvatarActor;
}

bool UInventorySystemComponent::GetInventorySlotForItem(UItem* Item, FInventorySlotData& InventorySlot)
{

	if(const FInventorySlotData* Slot = InventoryMap.Find(Item))
	{
		InventorySlot = *Slot;
		return true;
	}

	InventorySlot = FInventorySlotData(0);
	return false;
}

bool UInventorySystemComponent::TryEquipItem(UItem* Item, FEquippedSlot OptionalSlot)
{
	if(!Item)
	{
		return false;
	}

	FEquippedSlot Slot = OptionalSlot;

	if(!Slot.IsValidForItem(Item))
	{
		GetFirstAvailableEquipmentSlot(Item->GetItemType(), Slot);
	}

	if(!Slot.IsValid())
	{
		return false;
	}

	if(!EquipmentMap.Contains(Slot))
	{
		return false;
	}

	// See if we need to remove the item from our slot before continuing
	if(GetItemAtEquipmentSlot(Slot))
	{
		RemoveItemFromEquipmentSlot(Slot);
	}

	AddItemToEquipmentSlot(Slot, Item);
	return true;
}

bool UInventorySystemComponent::UseItemAtEquipmentSlot(const FEquippedSlot EquippedSlot)
{
	UItem* Item = GetItemAtEquipmentSlot(EquippedSlot);
	if(!Item)
	{
		return false;
	}

	OnEquipmentSlotUsed.Broadcast(EquippedSlot, Item);
	if(Item->ConsumeOnUse())
	{
		RemoveItem(Item);
	}

	return true;
}

int UInventorySystemComponent::GetTotalEquipmentSlotsOfType(FPrimaryAssetType Type)
{
	if(!Type.IsValid())
	{
		return 0;
	}

	int HighestValue = -1;

	for(TPair<FEquippedSlot, UItem*>& Pair : EquipmentMap)
	{
		if(Type == Pair.Key.SlotType)
		{
			if(Pair.Key.SlotNumber > HighestValue)
			{
				HighestValue = Pair.Key.SlotNumber;
			}
		}
	}

	return HighestValue;
}

UItem* UInventorySystemComponent::GetItemAtEquipmentSlot(const FEquippedSlot& EquippedSlot)
{
	UItem** FoundItem = EquipmentMap.Find(EquippedSlot);
	return FoundItem ? *FoundItem : nullptr;
}

bool UInventorySystemComponent::IsItemEquipped(const UItem* Item, FEquippedSlot& EquippedSlot)
{
	if(!Item)
	{
		EquippedSlot = FEquippedSlot();
		return false;
	}

	for(const TPair<FEquippedSlot, UItem*>& Pair : EquipmentMap)
	{
		if(Pair.Value == Item)
		{
			EquippedSlot = Pair.Key;
			return true;
		}
	}

	EquippedSlot = FEquippedSlot();
	return false;
}

bool UInventorySystemComponent::GetFirstAvailableEquipmentSlot(FPrimaryAssetType Type, FEquippedSlot& OutOpenSlot)
{
	if(!Type.IsValid())
	{
		return false;
	}

	for(TPair<FEquippedSlot, UItem*>& Slot : EquipmentMap)
	{
		FEquippedSlot& Key = Slot.Key;
		if(Type == Key.SlotType && !Slot.Value)
		{
			OutOpenSlot = Slot.Key;
			return true;
		}
	}

	OutOpenSlot = FEquippedSlot();
	return false;
}

void UInventorySystemComponent::AddItemToEquipmentSlot(const FEquippedSlot& EquippedSlot, UItem* Item)
{
	EquipmentMap.Add(EquippedSlot, Item);
	OnEquipmentSlotChanged.Broadcast(EquippedSlot, Item, EEquipmentSlotChangeType::Added);
}

void UInventorySystemComponent::RemoveItemFromEquipmentSlot(const FEquippedSlot& EquippedSlot)
{
	UItem* Item = *(EquipmentMap.Find(EquippedSlot));
	EquipmentMap.Add(EquippedSlot, nullptr);
	OnEquipmentSlotChanged.Broadcast(EquippedSlot, Item, EEquipmentSlotChangeType::Removed);
}

FOnEquipmentSlotChanged& UInventorySystemComponent::GetEquipmentSlotChangedDelegate()
{
	return OnEquipmentSlotChanged;
}
