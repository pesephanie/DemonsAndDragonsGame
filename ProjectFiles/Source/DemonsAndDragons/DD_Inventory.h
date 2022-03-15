// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DD_Inventory.generated.h"

UENUM()
enum ItemType {
	HealthPotion,
	BossDungeonKey,
	None
};

USTRUCT(BlueprintType)
struct FInventoryItem {
	GENERATED_BODY()

	/*Type of object*/
	UPROPERTY()
	TEnumAsByte<ItemType> Item;

	/*Number of object*/
	UPROPERTY()
	int Quantity;

	/*Default Constructor*/
	FInventoryItem() {
		Item = None;
		Quantity = 0;
	}
};

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class DEMONSANDDRAGONS_API UDD_Inventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDD_Inventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/*
	* The number of slots this inventory has
	* Currently linked only to hotbar so do not make higher than 10
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int SlotCount{ 10 };

	/*The contents of this inventory*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryItem> Items;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/*
	* Use item in inventory
	* @param ItemSlot= Slot of the item to use
	* @param Quantity= Quantity of item to use (default 1)
	* returns 'true' if successfull
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(int ItemSlot, int Quantity);
	
	/*
	* Add item to inventory - will add to slot containing identical item if any
	* @param Type= Object identifier
	* @param Quantity= Quantity to add (default 1)
	* returns 'false' if no slot was available for the item
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddInventoryItem(TEnumAsByte<ItemType> Type, int Quantity);

	/*
	* Get the item type of the item in the selected inventory slot
	* @param Slot= Slot to get the item from (0 -> SlotCount-1)
	* returns 'ItemType' of 'Slot' or 'None' if 'Slot' out of range
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	ItemType GetItemTypeInSlot(int Slot);

	/*
	* Get the quantity of items in the selected inventory slot
	* @param Slot= Slot to get the item from (0 -> SlotCount-1)
	* returns 'quantity' of 'item' in 'Slot' or '0' if 'Slot' out of range
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int GetItemCountInSlot(int Slot);

	/*
	* Implement this event to do whatever you like when the health potion is used
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void UseHealthPotion();

	/*
	* Implement this event to do whatever you like when the boss dungeon key is used
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void UseBossDungeonKey();

	/*
	* Override to define rules for when health potion sohuld be used
	* Return 'true' when you want to use it and 'false' when you want to prevent use
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	bool CheckHealthPotion();

	/*
	* Override to define rules for when boss dungeon key sohuld be used
	* Return 'true' when you want to use it and 'false' when you want to prevent use
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	bool CheckBossDungeonKey();

	/*
	* Override to define behaviour when inventory slots are updated
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void UpdateHUD();

private:
	/*Perform the effect of the selected inventory item*/
	UFUNCTION()
	void DoItemEffect(TEnumAsByte<ItemType> Type);

	/*Perform item check functions as defined by blueprint*/
	UFUNCTION()
	bool DoItemCheck(TEnumAsByte<ItemType> Type);
};
