// Fill out your copyright notice in the Description page of Project Settings.


#include "DD_EnemyCharacterBase.h"

// Sets default values
ADD_EnemyCharacterBase::ADD_EnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADD_EnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADD_EnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADD_EnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

