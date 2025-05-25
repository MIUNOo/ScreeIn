// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SCREEIN_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void LookAt(const UObject* Interactor, const APlayerController* Controller, const UPrimitiveComponent* HitComponent,
	AActor*& LookAtActor, UPrimitiveComponent*& LookAtComponent, FString& Message);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void InteractWith(const UObject* Interactor, const UPrimitiveComponent* HitComponent, bool& Interactable);
};
