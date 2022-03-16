// Fill out your copyright notice in the Description page of Project Settings.


#include "DD_PressurePlate.h"
#include "DD_PuzzleCrate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADD_PressurePlate::ADD_PressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PressurePlateBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pressure Plate Base Mesh"));
	SetRootComponent(PressurePlateBaseMesh);

	PlateSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Plate Mesh Location Reference"));
	PlateSceneComponent->SetupAttachment(PressurePlateBaseMesh);

	PressurePlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pressure Plate Mesh"));
	PressurePlateMesh->SetupAttachment(PlateSceneComponent);

	PressureVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Pressure Volume"));
	PressureVolume->SetupAttachment(PlateSceneComponent);
}

// Called when the game starts or when spawned
void ADD_PressurePlate::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADD_PressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePressureWeight();

	if (!bTriggered) {
		if (CurrentWeightOnPlate >= TriggerWeight) {
			bTriggered = true;
			TriggerEffectStart();
		}
	}
	else {
		if (CurrentWeightOnPlate < TriggerWeight) {
			bTriggered = false;
			TriggerEffectEnd();
		}
	}
}

void ADD_PressurePlate::UpdatePressureWeight() {
	// Flush weight and recalculate
	CurrentWeightOnPlate = 0.f;
	if (PressureCrateList.Num() > 0) {
		for (auto crate : PressureCrateList) {
			CurrentWeightOnPlate += crate->GetCrateWeight();
		}
	}
}

void ADD_PressurePlate::AddCrate(ADD_PuzzleCrate* CrateRef) {
	for (auto c : PressureCrateList)
	{
		if (c->GetName() == CrateRef->GetName()) // Contains this crate already
			return;
	}
	PressureCrateList.Add(CrateRef);
}

void ADD_PressurePlate::RemoveCrate(ADD_PuzzleCrate* CrateRef) {
	int index{ -1 };
	for (int i{ 0 }; i < PressureCrateList.Num(); i++) {
		if (PressureCrateList[i]->GetName() == CrateRef->GetName()) {
			index = i;
			break;
		}
	}
	if(index != -1)
		PressureCrateList.RemoveAt(index);
}