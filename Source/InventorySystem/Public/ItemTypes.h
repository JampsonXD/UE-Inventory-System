// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemTypes.generated.h"

class UInventorySystemComponent;
class UItem;

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemStateData
{
	GENERATED_BODY()

	FItemStateData()
	{
		Magnitude = 0.f;
		OptionalObject = nullptr;
	}

	bool operator==(FItemStateData& Other) const { return this->Magnitude == Other.Magnitude && this->OptionalObject == Other.OptionalObject; }
	bool operator!=(FItemStateData& Other) const { return !(*this == Other); }

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Magnitude;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* OptionalObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector LocationData;
};

USTRUCT(BlueprintType)
struct FEquippedSlot
{
	GENERATED_BODY()

	bool operator==(const FEquippedSlot& Other) const
	{
		return SlotType == Other.SlotType && SlotNumber == Other.SlotNumber;
	}

	FEquippedSlot()
	{
		SlotType = FPrimaryAssetType();
		SlotNumber = -1;
	}

	FEquippedSlot(FPrimaryAssetType Type, int InSlotNumber)
	{
		SlotType = Type;
		SlotNumber = InSlotNumber;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPrimaryAssetType SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SlotNumber;

	friend inline uint32 GetTypeHash(const FEquippedSlot& Key)
	{
		uint32 Hash = 0;
		Hash = HashCombine(Hash, GetTypeHash(Key.SlotType));
		Hash = HashCombine(Hash, static_cast<uint32>(Key.SlotNumber));
		return Hash;
	}

	bool IsValid() const
	{
		return SlotType.IsValid() && SlotNumber >= 0;
	}

	bool IsValidForItem(const UItem* Item) const
	{
		return Item && Item->ItemType == this->SlotType && SlotNumber >= 0;
	}
};

USTRUCT(BlueprintType)
struct FDefaultInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEquipOnAdded;
};

USTRUCT(BlueprintType)
struct FInventorySlotData
{
	GENERATED_BODY()
	
	FInventorySlotData()
	{
		StackCount = -1;
		ItemData = FItemStateData();
	}

	FInventorySlotData(int InStackCount, FItemStateData InItemData = FItemStateData())
	{
		StackCount = InStackCount;
		ItemData = InItemData;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemStateData ItemData;

	bool IsValid() const { return StackCount > 0;  }
	bool operator==(FInventorySlotData& Other) const { return this->StackCount == Other.StackCount && this->ItemData == Other.ItemData; }
	bool operator!=(FInventorySlotData& Other) const { return !(*this == Other); }

	void UpdateSlotData(const FInventorySlotData& Other, int MaxCount)
	{
		if(MaxCount < 0)
		{
			MaxCount = INT16_MAX;
		}

		StackCount = FMath::Clamp(StackCount + Other.StackCount, 0, MaxCount);
	}
};

UENUM(BlueprintType)
enum class EInventorySlotChangeType : uint8
{
	None,
	Removed,
	Added,
	StackChange
};

UENUM(BlueprintType)
enum class EEquipmentSlotChangeType : uint8
{
	None,
	Added,
	Removed
};