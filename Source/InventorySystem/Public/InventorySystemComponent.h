// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemTypes.h"
#include "Components/ActorComponent.h"
#include "InventorySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChanged, UItem*, Item, EInventorySlotChangeType, ChangeType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemStackCountChanged, int, OldStackCount, int, NewStackCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquipmentSlotChanged, FEquippedSlot, EquippedSlotData, UItem*, Item, EEquipmentSlotChangeType, ChangeType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentSlotUsed, FEquippedSlot, EquippedSlot, UItem*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()


	// Owning actor of our component
	UPROPERTY()
	AActor* OwningActor;

	// Actor that our owning actor maybe controlling. Can be the same as our owning actor
	UPROPERTY()
	AActor* AvatarActor;
	
protected:

	// Map of Item Data to Inventory Data such as stack count
	UPROPERTY(BlueprintReadOnly, Category = "Inventory System Component | Items")
	TMap<UItem*, FInventorySlotData> InventoryMap;
	
	/* Array of Item Classes to grant our Component when initialized
	adding one or more of the same class will add a new item or update the existing
	stack count of an item based on its stackable setting
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory System Component | Defaults")
	TArray<FDefaultInventoryData> DefaultInventoryItemData;

	UPROPERTY(BlueprintAssignable)
	FOnItemChanged OnItemChanged;

public:

	UPROPERTY()
	TMap<const UItem*, FOnItemStackCountChanged> ItemStackCountChangedMap;

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component")
	AActor* GetOwningActor() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component")
	AActor* GetAvatarActor() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Items")
	bool AddItem(UItem* Item, int StackCount = 1, bool bAutoEquip = false);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Items")
	bool RemoveItem(UItem* Item, int StackCount = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Items")
	bool SetItemStateData(UItem* Item, FItemStateData ItemStateData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Component | Items")
	FItemStateData GetItemStateData(UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Items")
	bool GetInventoryItems(FPrimaryAssetType ItemType, TArray<UItem*>& OutItems);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Items")
	int GetItemStackCount(const UItem* Item);

	/** Initializes our Default Inventory Items
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Defaults")
	void InitInventorySystemComponent();

	/* Returns true or false based on if this inventory has an instance of this item */
	UFUNCTION(BlueprintCallable, Category = "Inventory System Component")
	bool HasItem(const UItem* Item);

	UFUNCTION()
	FOnItemChanged& GetOnItemChangedDelegate();

	UFUNCTION()
	FOnItemStackCountChanged& RegisterItemStackCountChangedEvent(const UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Initialize")
	void InitActorInfo(AActor* InOwningActor, AActor* InAvatarActor);

protected:

	UFUNCTION()
	bool GetInventorySlotForItem(UItem* Item, FInventorySlotData& InventorySlot);


	/**********************************************************
	 ***                  Equipment Slots                  ****
	 *********************************************************/

protected:

	// Maps equipped slot data to the item that is stored within
	UPROPERTY(BlueprintReadOnly, Category = "Inventory System Component | Equipment")
	TMap<FEquippedSlot, UItem*> EquipmentMap;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotChanged OnEquipmentSlotChanged;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotUsed OnEquipmentSlotUsed;

	/* Default map of item slot types and how many slots to apply for each type when our component is initialized */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory System Component | Equipment")
	TMap<FPrimaryAssetType, int32> DefaultEquipmentSlots;

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Equipment")
	bool TryEquipItem(UItem* Item, FEquippedSlot OptionalSlot = FEquippedSlot());

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Equipment")
	bool UseItemAtEquipmentSlot(const FEquippedSlot EquippedSlot);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Equipment")
	int GetTotalEquipmentSlotsOfType(FPrimaryAssetType Type);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Equipment")
	bool IsItemEquipped(const UItem* Item, FEquippedSlot& EquippedSlot);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Equipment")
	UItem* GetItemAtEquipmentSlot(const FEquippedSlot& EquippedSlot);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Equipment")
	bool GetFirstAvailableEquipmentSlot(FPrimaryAssetType Type, FEquippedSlot& OutOpenSlot);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Equipment")
	void AddItemToEquipmentSlot(const FEquippedSlot& EquippedSlot, UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Equipment")
	void RemoveItemFromEquipmentSlot(const FEquippedSlot& EquippedSlot);

	UFUNCTION()
	FOnEquipmentSlotChanged& GetEquipmentSlotChangedDelegate();
};
