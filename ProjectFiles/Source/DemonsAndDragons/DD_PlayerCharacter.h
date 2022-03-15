// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DD_PlayerCharacter.generated.h"

UCLASS()
class DEMONSANDDRAGONS_API ADD_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADD_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/*User Input*/

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player") // Not allowed by the engine!
	class IDD_InteractableObjectInterface* InteractedObject;

	UPROPERTY()
	FRotator MeshInitialRotation{};

	UFUNCTION()
	void MoveForwardBackward(float inVal);

	UFUNCTION()
	void MoveRightLeft(float inVal);

	UFUNCTION()
	void LookUpDown(float inVal);

	UFUNCTION()
	void LookRightLeft(float inVal);

	UFUNCTION()
	void InteractWithObject();

	UFUNCTION()
	void StopInteract();
	
	/*
	* Checks whether the interactable object is within view of the player camera
	*/
	UFUNCTION()
	bool CanReachInteractable(class AActor* InteractableObject);
	
	
	/****DEBUG****/
	/*FVector LineStart{};
	FVector LineEnd{};*/

protected:
	/*Custom Components*/

	/*Spring arm - Used to hold camera component at an angle from the player - inherits from scene component also*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class USpringArmComponent* CameraSpringArm;

	/*Camera Component - for player viewport*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class UCameraComponent* PlayerCamera;

	/*Collision volume used to determine if there is an interactible object within player's reach*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class UBoxComponent* ObjectInteractVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MovementSensitivity{ 1.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MovementDeadzone{ 0.1f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float CameraSensitivity{ 1.f };

	/*Component referenceing where the carried object should be*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class USceneComponent* CarryObjectRef;

	/*The object currently being carried - if any*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class AActor* CarriedObject;

	/*True when carrying object*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	bool bIsCarrying{ false };

public:
	/*Custom Functions*/

	/*
	* Returns true if object was successfully attached to player
	* Returns false if there is already an object attached
	*/
	UFUNCTION()
	bool AttachCarryObject(class AActor* ActorToAttach);

	/*
	* Cancels attachment of object if there is one
	*/
	UFUNCTION()
	bool DetachCarryObject();
};