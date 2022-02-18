// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DD_InteractableObjectInterface.h"
#include "DD_Lever.generated.h"

UCLASS()
class DEMONSANDDRAGONS_API ADD_Lever : public AActor, public IDD_InteractableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADD_Lever();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lever")
	class UStaticMeshComponent* LeverArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lever")
	class UStaticMeshComponent* LeverBase;

	/*
	* Called whenever the lever switches state
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Lever")
	void LeverSwitched();

	/*
	* True if the lever is up
	* False if lever is down
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lever")
	const bool GetLeverState();

private:

	UPROPERTY()
	bool bUp{ false };

public:	
	// Called every frame
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

	// Called when player interacts with the lever
	UFUNCTION()
	virtual void InteractWith(ADD_PlayerCharacter* playerCharacter) override;
};
