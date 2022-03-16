// Fill out your copyright notice in the Description page of Project Settings.


#include "DD_PuzzleCrate.h"
#include "DD_PlayerCharacter.h"
#include "DD_PressurePlate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADD_PuzzleCrate::ADD_PuzzleCrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CrateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Moveable Crate Mesh"));
	SetRootComponent(CrateMesh);

	WeightingCollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Weighting Collision Volume"));
	WeightingCollisionVolume->SetupAttachment(CrateMesh);

	// Setup delegate functions for binding
	BeginCrateOL.BindUFunction(this, "BeginOverlapWithOther");
	EndCrateOL.BindUFunction(this, "EndOverlapWithOther");
}

// Called when the game starts or when spawned
void ADD_PuzzleCrate::BeginPlay()
{
	Super::BeginPlay();

	// Bind Delegates
	WeightingCollisionVolume->OnComponentBeginOverlap.Add(BeginCrateOL);
	WeightingCollisionVolume->OnComponentEndOverlap.Add(EndCrateOL);
}

// Called every frame
void ADD_PuzzleCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADD_PuzzleCrate::InteractWith(ADD_PlayerCharacter* PlayerCharacter) {
	PlayerCharacterRef = PlayerCharacter;
	CrateMesh->SetSimulatePhysics(false); // For the sake of simplicity
	PlayerCharacterRef->AttachCarryObject(this);
	PickUp();
}

void ADD_PuzzleCrate::StopInteraction() {
	if (PlayerCharacterRef) {
		PlayerCharacterRef->DetachCarryObject();
		PlayerCharacterRef = nullptr; // Unlink from carrying player
		CrateMesh->SetSimulatePhysics(true); // Just re-enable to be safe
	}
	PutDown();
}

void ADD_PuzzleCrate::BeginOverlapWithOther(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {

	ADD_PressurePlate* Plate{ Cast<ADD_PressurePlate>(OtherActor) };
	if (Plate != nullptr) {
		//UE_LOG(LogTemp, Warning, TEXT("%s: Begin overlap with plate: %s"), *GetName(), *Plate->GetName());
		PlateRef = Plate;
		PlateRef->AddCrate(this);
		PlateRef->WeightChanged();
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Crate Begin Overlap: Plate Invalid"));

	ADD_PuzzleCrate* OtherCrate{ Cast<ADD_PuzzleCrate>(OtherActor) };
	if (OtherCrate != nullptr) {
		//UE_LOG(LogTemp, Warning, TEXT("%s: Begin overlap with crate: %s"), *GetName(), *OtherCrate->GetName());
		if (PlateRef == nullptr && OtherCrate->PlateRef != nullptr) {
			PlateRef = OtherCrate->PlateRef;
			PlateRef->AddCrate(this);
			PlateRef->WeightChanged();
			return;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Crate Begin Overlap: Other Crate Invalid/Plate Ref Already Valid/Other Crate Plate Ref Invalid"));
}

void ADD_PuzzleCrate::EndOverlapWithOther(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	
	ADD_PressurePlate* Plate{ Cast<ADD_PressurePlate>(OtherActor) };
	if (Plate != nullptr && PlateRef != nullptr) {
		//UE_LOG(LogTemp, Warning, TEXT("%s: End overlap with plate: %s"), *GetName(), *Plate->GetName());
		PlateRef->RemoveCrate(this);
		PlateRef->WeightChanged();
		PlateRef = nullptr;
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Crate End Overlap: Plate Invalid"));

	// Remove the weight of this crate from the pressure plate
	// ONLY if this is the crate being removed!
	ADD_PuzzleCrate* OtherCrate{ Cast<ADD_PuzzleCrate>(OtherActor) };
	if (OtherCrate != nullptr) {
		//UE_LOG(LogTemp, Warning, TEXT("%s: End overlap with crate: %s"), *GetName(), *OtherCrate->GetName());
		int count{ GetOverlappingWeightObjectCount() };
		if (count <= 1) {
			if (PlateRef != nullptr) {
				PlateRef->RemoveCrate(this);
				PlateRef = nullptr;
			}
		}
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Crate End Overlap: Other Crate Invalid"));
}

int ADD_PuzzleCrate::GetOverlappingWeightObjectCount() {
	int OLCount{ 0 };
	TSet<AActor*> OutActors;
	WeightingCollisionVolume->GetOverlappingActors(OutActors);
	for (AActor* act : OutActors) {
		if (Cast<ADD_PuzzleCrate>(act) != nullptr || Cast<ADD_PressurePlate>(act) != nullptr) {
			OLCount++;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Overlapping actor count for %s: %d"), *GetName(), OLCount);

	return OLCount;
}