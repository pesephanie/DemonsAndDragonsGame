// Fill out your copyright notice in the Description page of Project Settings.


#include "DD_Inventory.h"

// Sets default values for this component's properties
UDD_Inventory::UDD_Inventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDD_Inventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	for (int i{ 0 }; i < SlotCount; i++)
		Items.Add(FInventoryItem{});
	UE_LOG(LogTemp, Warning, TEXT("Inventory initialized with %f slots"), Items.Num());
}


/*// Called every frame
void UDD_Inventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/

bool UDD_Inventory::UseItem(int ItemSlot, int Quantity) {
	if (Items[ItemSlot].Item == None) // Nothing in the slot
		return false;
	if (Items[ItemSlot].Quantity == 0) // Nothing in the slot - just in case the first one does not work
		return false;

	// Calculate how many items can be used (i.e. if more than available requested use only those available)
	int QuantityToUse{ (Items[ItemSlot].Quantity >= Quantity) ? Quantity : Items[ItemSlot].Quantity };
	
	// Use item if valid
	for (int i{ 0 }; i < QuantityToUse; i++) {
		if (DoItemCheck(Items[ItemSlot].Item)) {// Item check passed
			DoItemEffect(Items[ItemSlot].Item); // Do item effect
			Items[ItemSlot].Quantity--; // Remove item from slot
		}
	}

	// Set item type to none if no items remaining
	if (Items[ItemSlot].Quantity == 0)
		Items[ItemSlot].Item = None;
	
	UpdateHUD();
	return true;
}

bool UDD_Inventory::AddInventoryItem(TEnumAsByte<ItemType> Type, int Quantity) {
	bool InventoryHasItem{ false };
	bool InventoryHasSpace{ false };
	int SlotToAddTo{ 0 };

	// Check if inventory already has identical item
	for (int i{ Items.Num() - 1 }; i >= 0; i--) {
		if (Items[i].Item == Type) {
			InventoryHasItem = true;
			InventoryHasSpace = true; // No stack limit
			SlotToAddTo = i;
			break;
		}
		else if(Items[i].Item == None) {
			SlotToAddTo = i;
			InventoryHasSpace = true;
		}
	}

	// If there is space for the item in the inventory, add the item
	if (InventoryHasSpace) {
		if (InventoryHasItem) {
			Items[SlotToAddTo].Quantity += Quantity;
		}
		else {
			Items[SlotToAddTo].Item = Type;
			Items[SlotToAddTo].Quantity = Quantity;
		}
	}

	UpdateHUD();
	return InventoryHasSpace;
}

ItemType UDD_Inventory::GetItemTypeInSlot(int Slot = 0) {
	if (Slot < Items.Num())
		return Items[Slot].Item;
	return None;
}

int UDD_Inventory::GetItemCountInSlot(int Slot = 0) {
	if (Slot < Items.Num())
		return Items[Slot].Quantity;
	return 0;
}

void UDD_Inventory::DoItemEffect(TEnumAsByte<ItemType> Type) {
	switch (Type) {
	case HealthPotion:
		UseHealthPotion();
		break;
	case BossDungeonKey:
		UseBossDungeonKey();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("DD_Inventory->DoItemEffect(): Tried to perform effect on invalid ItemType!"));
		break;
	}
}

bool UDD_Inventory::DoItemCheck(TEnumAsByte<ItemType> Type) {
	switch (Type) {
	case HealthPotion:
		return CheckHealthPotion();
		break;
	case BossDungeonKey:
		return CheckBossDungeonKey();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("DD_Inventory->DoItemCheck(): Tried to perform check on invalid ItemType!"));
		break;
	}
	return false;
}