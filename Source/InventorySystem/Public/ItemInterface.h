// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYSYSTEM_API IItemInterface
{
	GENERATED_BODY()

public:

	/* The type of item this is */
	virtual FPrimaryAssetType GetItemType() const = 0;

	/* Whether the item is stackable or not */
	virtual bool IsStackable() const = 0;

	/* The max stack count of the item */
	virtual int GetMaxStackCount() const = 0;

	/* Whether the item should be consumed on use */
	virtual bool ConsumeOnUse() const = 0;

	/* Item to call when the item is used */
	//virtual void Use(AActor* OwningActor) const = 0;

	/* Function to call when the item is equipped */
	//virtual void OnEquipped(AActor* OwningActor) const = 0;
};
