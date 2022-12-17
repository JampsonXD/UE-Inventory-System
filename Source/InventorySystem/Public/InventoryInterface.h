// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYSYSTEM_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual AActor* GetOwningActor() const = 0;

	virtual AActor* GetAvatarActor() const = 0;

	virtual void InitializeInventory(AActor* InOwningActor, AActor* InAvatarActor) = 0;

	virtual bool TryAddItem(IItemInterface* Item, const int StackAmount) = 0;

	virtual bool TryRemoveItem(IItemInterface* Item, const int StackAmount) = 0;




};
