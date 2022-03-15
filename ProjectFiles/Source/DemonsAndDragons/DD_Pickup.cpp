// Fill out your copyright notice in the Description page of Project Settings.


#include "DD_Pickup.h"

// Sets default values
ADD_Pickup::ADD_Pickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADD_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADD_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADD_Pickup::InteractWith(ADD_PlayerCharacter* PlayerCharacter)
{
	OnPickUp();
}