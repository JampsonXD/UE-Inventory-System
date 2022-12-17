// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventorySystemInterface.generated.h"

class UInventorySystemComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventorySystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYSYSTEM_API IInventorySystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual UInventorySystemComponent* GetInventorySystemComponent() const = 0;
};
