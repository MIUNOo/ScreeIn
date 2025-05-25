// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockPlane.h"
#include "Engine/DataAsset.h"
#include "PickupItemData.generated.h"

UENUM()
enum class EPickupItemType
{
	Default,
	NumberButton,
	Key,
	BlockPlane,
};

/**
 * 
 */
UCLASS(BlueprintType)
class SCREEIN_API UPickupItemData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")	
	ECubeColor CubeColor = ECubeColor::Grey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	// bool bIsNumberButton = false;
	EPickupItemType ItemType = EPickupItemType::Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	int32 Number = 0;
};
