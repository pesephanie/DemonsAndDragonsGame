// Fill out your copyright notice in the Description page of Project Settings.


#include "DD_BananaTree.h"

// Sets default values
ADD_BananaTree::ADD_BananaTree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADD_BananaTree::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADD_BananaTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADD_BananaTree::InteractWith(ADD_PlayerCharacter* PlayerCharacter)
{
	OnShake();
}