// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UBFL_Inventory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Helpers")
    static class UInventorySystemComponent* GetInventorySystemComponent(AActor* InActor);
};
