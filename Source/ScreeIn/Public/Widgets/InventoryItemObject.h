// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryItemObject.generated.h"

class UPickupItemData;
/**
 * 
 */
UCLASS()
class SCREEIN_API UInventoryItemObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UPickupItemData* PickupItemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	bool bIsSelected;
};
