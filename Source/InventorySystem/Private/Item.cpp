// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

FPrimaryAssetType UItem::GetItemType() const
{
	return ItemType;
}

FName UItem::GetItemName() const
{
	return ItemName;
}

FText UItem::GetItemDescription() const
{
	return ItemDescription;
}

UTexture2D* UItem::GetItemImage() const
{
	return ItemImageSoftPointer;
}

bool UItem::IsStackable() const
{
	return bIsStackable;
}

int UItem::GetMaxStackCount() const
{
	return MaxStackCount;
}

bool UItem::ConsumeOnUse() const
{
	return bConsumeOnUse;
}

FString UItem::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
