// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItem.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class SCREEIN_API UInventoryItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	UImage* SelectedImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	UImage* ItemIcon;
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	// FColor InactiveColor;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	// FColor ActiveColor;
};
