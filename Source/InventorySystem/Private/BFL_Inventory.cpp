// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_Inventory.h"

#include "InventorySystemInterface.h"

UInventorySystemComponent* UBFL_Inventory::GetInventorySystemComponent(AActor* InActor)
{
	if(!InActor)
	{
		return nullptr;
	}

	IInventorySystemInterface* Interface = Cast<IInventorySystemInterface>(InActor);

	if(!Interface)
	{
		return nullptr;
	}

	return Interface->GetInventorySystemComponent();
}
