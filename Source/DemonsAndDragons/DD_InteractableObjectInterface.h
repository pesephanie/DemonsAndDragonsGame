// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DD_PlayerCharacter.h"
#include "DD_InteractableObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDD_InteractableObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface that any object requiring interaction with player character should implement
 * Must implement following functions:
 *	virtual void InteractWith() override;
 * Optional functions:
 *	virtual void StopInteraction() override;
 */
class DEMONSANDDRAGONS_API IDD_InteractableObjectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*playerCharacter reference to the player character interacting with this object*/
	virtual void InteractWith(ADD_PlayerCharacter* playerCharacter) abstract;
	virtual void StopInteraction() { return; } // Default implementation does nothing - here just in case you want to use it
};
