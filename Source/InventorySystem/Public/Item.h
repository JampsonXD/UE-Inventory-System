// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemInterface.h"
#include "Item.generated.h"

class UInventorySystemComponent;

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UItem : public UPrimaryDataAsset, public IItemInterface
{
	GENERATED_BODY()

friend UInventorySystemComponent;

public:

	// Label for our Item, what the Item Type is
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Info")
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Info")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Info")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Info")
	TSubclassOf<AActor> ItemInstanceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Info")
	UTexture2D* ItemImageSoftPointer;

	// If we can be stacked, our max stack count, defaults to -1 if we have unlimited stacks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Info")
	int32 MaxStackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Info")
	bool bConsumeOnUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Info")
	bool bIsStackable;

	virtual FPrimaryAssetType GetItemType() const override;

	virtual FName GetItemName() const;

	virtual FText GetItemDescription() const;

	virtual UTexture2D* GetItemImage() const;

	virtual bool IsStackable() const override;

	virtual int GetMaxStackCount() const override;

	virtual bool ConsumeOnUse() const override;

	UFUNCTION(BlueprintCallable, Category = "Item")
	FString GetIdentifierString() const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
