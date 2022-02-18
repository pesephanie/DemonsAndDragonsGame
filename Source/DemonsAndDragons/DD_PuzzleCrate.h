// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DD_InteractableObjectInterface.h"
#include "DD_PuzzleCrate.generated.h"

UCLASS()
class DEMONSANDDRAGONS_API ADD_PuzzleCrate : public AActor, public IDD_InteractableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADD_PuzzleCrate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crate")
	class UStaticMeshComponent* CrateMesh;
	
	/*
	* Used to check if any other box component is colliding with this one
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crate")
	class UBoxComponent* WeightingCollisionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crate")
	float Weight{ 10.f }; // Basic weight of crates

	/*
	* Event called when InteractWith() is called on this object
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Crate")
	void PickUp();

	/*
	* Event called when StopInteraction() is called on this object
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Crate")
	void PutDown();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void InteractWith(ADD_PlayerCharacter* PlayerCharacter) override;

	UFUNCTION()
	virtual void StopInteraction() override;

	UFUNCTION()
	float GetCrateWeight() { return Weight; }

private:
	UPROPERTY()
	ADD_PlayerCharacter* PlayerCharacterRef;

	/*Custom Delegates*/
	TScriptDelegate<FWeakObjectPtr> BeginCrateOL;
	TScriptDelegate<FWeakObjectPtr> EndCrateOL;

	UFUNCTION()
	void BeginOverlapWithCrate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlapWithCrate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*
	* Pressure plate reference object
	*/
	UPROPERTY()
	class ADD_PressurePlate* PlateRef{ nullptr };
};