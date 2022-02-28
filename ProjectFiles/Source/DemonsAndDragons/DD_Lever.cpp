// Fill out your copyright notice in the Description page of Project Settings.


#include "DD_Lever.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimSequence.h"

// Sets default values
ADD_Lever::ADD_Lever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LeverBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever Base"));
	SetRootComponent(LeverBase);

	LeverArm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever Arm"));
	LeverArm->SetupAttachment(LeverBase);
}

// Called when the game starts or when spawned
void ADD_Lever::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADD_Lever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const bool ADD_Lever::GetLeverState() {
	return bUp;
}

void ADD_Lever::InteractWith(ADD_PlayerCharacter* playerCharacter) {
	UE_LOG(LogTemp, Warning, TEXT("Player pulled the lever"));
	bUp = !bUp;
	LeverSwitched();
}