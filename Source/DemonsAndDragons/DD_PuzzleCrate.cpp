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
	BeginCrateOL.BindUFunction(this, "BeginOverlapWithCrate");
	EndCrateOL.BindUFunction(this, "EndOverlapWithCrate");
}

// Called when the game starts or when spawned
void ADD_PuzzleCrate::BeginPlay()
{
	Super::BeginPlay();

	// Bind Delegates
	WeightingCollisionVolume->OnComponentBeginOverlap.AddUnique(BeginCrateOL);
	WeightingCollisionVolume->OnComponentEndOverlap.AddUnique(EndCrateOL);
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

void ADD_PuzzleCrate::BeginOverlapWithCrate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {

	ADD_PressurePlate* Plate{ Cast<ADD_PressurePlate>(OtherActor) };
	if (Plate != nullptr) {
		PlateRef = Plate;
		PlateRef->AddCrate(this);
		return;
	}

	ADD_PuzzleCrate* OtherCrate{ Cast<ADD_PuzzleCrate>(OtherActor) };
	if (OtherCrate != nullptr && PlateRef == nullptr && OtherCrate->PlateRef != nullptr) {
		PlateRef = OtherCrate->PlateRef;
		PlateRef->AddCrate(this);
		return;
	}
}

void ADD_PuzzleCrate::EndOverlapWithCrate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	
	ADD_PressurePlate* Plate{ Cast<ADD_PressurePlate>(OtherActor) };
	if (Plate != nullptr) {
		PlateRef->RemoveCrate(this);
		//PlateRef = nullptr;
		return;
	}

	ADD_PuzzleCrate* OtherCrate{ Cast<ADD_PuzzleCrate>(OtherActor) };
	if (OtherCrate != nullptr) {
		if (PlateRef != nullptr) {
			PlateRef->RemoveCrate(this);
			//PlateRef = nullptr;
		}
		return;
	}
}