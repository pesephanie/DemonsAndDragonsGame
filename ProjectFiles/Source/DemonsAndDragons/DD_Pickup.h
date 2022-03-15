// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DD_InteractableObjectInterface.h"
#include "DD_Pickup.generated.h"

UCLASS()
class DEMONSANDDRAGONS_API ADD_Pickup : public AActor, public IDD_InteractableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADD_Pickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* Event called when InteractWith() is called on this object
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnPickUp();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void InteractWith(ADD_PlayerCharacter* PlayerCharacter) override;
};
