// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DD_PressurePlate.generated.h"

UCLASS()
class DEMONSANDDRAGONS_API ADD_PressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADD_PressurePlate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Custom Blueprint Events
	
	/*
	* Called once there is sufficient mass 'on' the plate
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Pressure Plate")
	void TriggerEffectStart();

	/*
	* Called when there is no sufficient mass 'on' the plate
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Pressure Plate")
	void TriggerEffectEnd();

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pressure Plate")
	class UStaticMeshComponent* PressurePlateMesh;

	/*
	* The collision volume that must contain enough weighted objects for the plate to trigger
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pressure Plate")
	class UBoxComponent* PressureVolume;

	/*
	* The total weight required on this plate to trigger the active state
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pressure Plate")
	float TriggerWeight{ 20.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pressure Plate")
	float CurrentWeightOnPlate{ 0.f };

private:

	/*
	* Current trigger state
	*/
	UPROPERTY()
	bool bTriggered{ false };

	/*
	* List of all crates putting pressure on this plate
	*/
	UPROPERTY()
	TArray<class ADD_PuzzleCrate*> PressureCrateList{};

	/*
	* Flush and refresh plate list as required
	* Also updates CurrentWeightOnPlate
	*/
	UFUNCTION()
	void UpdatePressureWeight();

public:
	/*
	* Function that is called from crate when adding to list
	*/
	UFUNCTION()
	void AddCrate(class ADD_PuzzleCrate* CrateRef);

	/*
	* Function that is called from crate when adding to list
	*/
	UFUNCTION()
	void RemoveCrate(class ADD_PuzzleCrate* CrateRef);
};
