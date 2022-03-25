// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DD_InteractableObjectInterface.h"
#include "DD_BananaTree.generated.h"

UCLASS()
class DEMONSANDDRAGONS_API ADD_BananaTree : public AActor, public IDD_InteractableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADD_BananaTree();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnShake();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void InteractWith(ADD_PlayerCharacter* PlayerCharacter) override;
};
