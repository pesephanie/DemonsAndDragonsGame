// Fill out your copyright notice in the Description page of Project Settings.

#define Collision_Interactable ECC_GameTraceChannel1 // InteractableObject

// DD Includes
#include "DD_PlayerCharacter.h"
#include "DD_InteractableObjectInterface.h"

// UE4 includes
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADD_PlayerCharacter::ADD_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->TargetArmLength = 300.f;
	CameraSpringArm->TargetOffset = FVector{0.f, 20.f, 30.f};

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Player Camera"));
	PlayerCamera->SetupAttachment(CameraSpringArm);

	ObjectInteractVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Object Interact Volume"));
	ObjectInteractVolume->SetupAttachment(GetMesh());
	ObjectInteractVolume->SetCollisionProfileName(TEXT("Interactable"));
	ObjectInteractVolume->SetRelativeLocation(FVector{ 88.f, 0, 88.f });
	ObjectInteractVolume->SetBoxExtent(FVector{ 88.f, 88.f, 88.f });

	CarryObjectRef = CreateDefaultSubobject<USceneComponent>(TEXT("Carry Object Reference"));
	CarryObjectRef->SetupAttachment(CameraSpringArm);
}

// Called when the game starts or when spawned
void ADD_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	MeshInitialRotation = GetMesh()->GetRelativeRotation();
}

// Called every frame
void ADD_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCarrying && CarriedObject) {
		CarriedObject->SetActorLocation(CarryObjectRef->GetComponentLocation(), true);
	}
	//DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor{124, 252, 0, 255});
}

// Called to bind functionality to input
void ADD_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ADD_PlayerCharacter::MoveForwardBackward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ADD_PlayerCharacter::MoveRightLeft);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ADD_PlayerCharacter::LookUpDown);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ADD_PlayerCharacter::LookRightLeft);

	// Interaction
	PlayerInputComponent->BindAction(TEXT("ObjectInteract"), EInputEvent::IE_Pressed, this, &ADD_PlayerCharacter::InteractWithObject);
	PlayerInputComponent->BindAction(TEXT("ObjectInteract"), EInputEvent::IE_Released, this, &ADD_PlayerCharacter::StopInteract);
}

void ADD_PlayerCharacter::MoveForwardBackward(float inVal) {
	if (FMath::Abs(inVal) > MovementDeadzone) {
		//TurnForward();
		AddMovementInput(CameraSpringArm->GetForwardVector(), inVal * MovementSensitivity);
	}
}

void ADD_PlayerCharacter::MoveRightLeft(float inVal) {
	if (FMath::Abs(inVal) > MovementDeadzone) {
		//TurnForward();
		AddMovementInput(CameraSpringArm->GetRightVector(), inVal * MovementSensitivity);
	}
}

void ADD_PlayerCharacter::LookUpDown(float inVal) {
	// Update camera spring arm
	FRotator rot = CameraSpringArm->GetComponentRotation();
	rot += FRotator{ inVal * CameraSensitivity, 0.f, 0.f };
	rot.Roll = 0.f;
	CameraSpringArm->SetWorldRotation(rot);

	//AddControllerPitchInput(inVal * CameraSensitivity);
}

void ADD_PlayerCharacter::LookRightLeft(float inVal) {
	// Update camera spring arm
	/*FRotator rot = CameraSpringArm->GetComponentRotation();
	rot += FRotator{ 0.f, inVal * CameraSensitivity, 0.f};
	rot.Roll = 0.f;
	CameraSpringArm->SetWorldRotation(rot);*/

	AddControllerYawInput(inVal * CameraSensitivity);
}

void ADD_PlayerCharacter::TurnForward() {
	FRotator newRot{}; // {GetMesh()->GetComponentRotation()};
	newRot.Pitch = 0.f;
	newRot.Yaw = CameraSpringArm->GetComponentRotation().Yaw;
	newRot.Roll = 0.f;
	newRot += MeshInitialRotation;
	GetMesh()->SetWorldRotation(newRot);
}

/*
* When pressed, interacts only with one 'interactable object' the player
* Selects the interactable object based on camera position from player - or nearest interactable object in reach
* WIP: Still requires a linetrace to closest object to ensure that no obstacle is between player and interactable object
*	This means that at the moment players can e.g. pull levers on the other side of obstacles etc.
*/
void ADD_PlayerCharacter::InteractWithObject() {
	UE_LOG(LogTemp, Warning, TEXT("Attempting to interact with object"));
	TSet<AActor*> DetectedActors;
	ObjectInteractVolume->GetOverlappingActors(DetectedActors);

	if (DetectedActors.Num() > 0) { // Collision box is overlapping any actors
		UE_LOG(LogTemp, Warning, TEXT("Overlapping actor count: %d"), DetectedActors.Num());
		TSet<AActor*> ValidDetectedActors;
		for (AActor* act : DetectedActors) {
			// check if act implements interface
			IDD_InteractableObjectInterface* actInt = Cast<IDD_InteractableObjectInterface>(act); // Cast to interface
			if (actInt) {
				ValidDetectedActors.Add(act);
			}
		}
		if (ValidDetectedActors.Num() > 0) { // If any actors collided with implement interactable-object-interface
			AActor* NearestActor{ nullptr }; // Nearest valid actor
			AActor* AimedActor{ nullptr }; // Actor that the player is pointing towards

			// Get neareset actor
			for (AActor* validActor : ValidDetectedActors) {
				if (NearestActor == nullptr) {
					NearestActor = validActor;
				}
				else {
					NearestActor = (GetDistanceTo(NearestActor) < GetDistanceTo(validActor)) ? NearestActor : validActor;
				}
			}

			// Get aimed at actor - using nearest actor location relative to camera as range
			FHitResult HR;
			float TraceLength{ (ObjectInteractVolume->GetComponentLocation() - PlayerCamera->GetComponentLocation()).Size() }; // Dynamic length for ray trace
			TraceLength += ObjectInteractVolume->GetScaledBoxExtent().GetAbsMax(); // Add box extent to raytrace length - just so that you can reach it properly
			FVector HitEndPos{ (PlayerCamera->GetComponentRotation().Vector() * TraceLength) + PlayerCamera->GetComponentLocation() };
			FCollisionQueryParams queryParams{ FName{}, false, this }; // Must ignore this actor

			/*LineStart = PlayerCamera->GetComponentLocation();
			LineEnd = HitEndPos;*/

			bool bHit{ GetWorld()->LineTraceSingleByChannel(
					HR,
					PlayerCamera->GetComponentLocation(),
					HitEndPos,
					ECollisionChannel::Collision_Interactable,
					queryParams
				)
			};

			if (bHit) {
				AimedActor = Cast<AActor>(HR.Actor);
			}

			// Interact with aimed actor - or if none, interact with nearest actor - only if visible to player
			if (AimedActor && CanReachInteractable(AimedActor))
				InteractedObject = Cast<IDD_InteractableObjectInterface>(AimedActor); // Check aimed at actor

			if (!InteractedObject && CanReachInteractable(NearestActor))
				InteractedObject = Cast<IDD_InteractableObjectInterface>(NearestActor); // Check nearest actor

			if (InteractedObject)
				InteractedObject->InteractWith(this); // Call 'interact' function only on valid actor
		}
	}
}

/*
* Cancel interaction with object currently being interacted with
*/
void ADD_PlayerCharacter::StopInteract() {
	if (InteractedObject) {
		InteractedObject->StopInteraction();
		InteractedObject = nullptr; // Unlink object from player once interaction is over
	}
}

/*
* Checks whether the interactable object is within view of the player camera
*/
bool ADD_PlayerCharacter::CanReachInteractable(class AActor* InteractableObject) {

	FHitResult HR{};
	FCollisionQueryParams QP{ FName{}, true, this };

	GetWorld()->LineTraceSingleByChannel(
		HR,
		GetActorLocation(),
		InteractableObject->GetActorLocation(),
		ECollisionChannel::ECC_Visibility,
		QP
	);

	if (HR.Actor == InteractableObject) {
		return true;
	}
	return false;
}

bool ADD_PlayerCharacter::AttachCarryObject(class AActor* ActorToAttach) {
	if (!ActorToAttach) // No valid actor passed in
		return false;
	if(CarriedObject) // There is already an object being carried
		return false;

	CarryObjectRef->SetWorldLocation(ActorToAttach->GetActorLocation());
	bIsCarrying = true;
	CarriedObject = ActorToAttach;
	return true;
}

bool ADD_PlayerCharacter::DetachCarryObject() {
	if (!CarriedObject) // No carried object to detach
		return false;

	bIsCarrying = false;
	CarriedObject = nullptr;
	return true;
}