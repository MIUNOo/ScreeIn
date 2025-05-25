// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UPickupItemData;
class UTileView;
/**
 * 
 */
UCLASS()
class SCREEIN_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	UTileView* InventoryItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxItemCount = 5;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void AddItem(UPickupItemData* ItemData);

	UFUNCTION()
	void RemoveItem(UPickupItemData* ItemData);

	UFUNCTION()
	void ChangeIndex(int32 Index);

private:
	void RefreshInventory();
	
	int32 CurrentIndex;
};
