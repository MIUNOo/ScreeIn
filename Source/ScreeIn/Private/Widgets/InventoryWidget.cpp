// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryWidget.h"

#include "ScreeInPlayerController.h"
#include "Components/ListView.h"
#include "Components/TileView.h"
#include "Widgets/InventoryItemObject.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	for (int i = 0; i < MaxItemCount; ++i)
	{
		UInventoryItemObject* InventoryItemObject = NewObject<UInventoryItemObject>();
		InventoryItems->AddItem(InventoryItemObject);
	}

	AScreeInPlayerController* ScreeInPlayerController = Cast<AScreeInPlayerController>(GetOwningPlayer());
	if (ScreeInPlayerController)
	{
		ScreeInPlayerController->OnInventoryAddItem.AddDynamic(this, &UInventoryWidget::AddItem);
		ScreeInPlayerController->OnInventoryRemoveItem.AddDynamic(this, &UInventoryWidget::RemoveItem);
		ScreeInPlayerController->OnInventoryIndexChange.AddDynamic(this, &UInventoryWidget::ChangeIndex);
	}
	
	CurrentIndex = -1;
}

void UInventoryWidget::AddItem(UPickupItemData* ItemData)
{
	RefreshInventory();
}

void UInventoryWidget::RemoveItem(UPickupItemData* ItemData)
{
	RefreshInventory();
}

void UInventoryWidget::ChangeIndex(int32 Index)
{
	UInventoryItemObject* InventoryItemObject = Cast<UInventoryItemObject>(InventoryItems->GetItemAt(CurrentIndex));
	InventoryItemObject->bIsSelected = false;
	
	InventoryItemObject = Cast<UInventoryItemObject>(InventoryItems->GetItemAt(Index));
	InventoryItemObject->bIsSelected = true;
	
	InventoryItems->RegenerateAllEntries();
	CurrentIndex = Index;
}

void UInventoryWidget::RefreshInventory()
{
	AScreeInPlayerController* ScreeInPlayerController = Cast<AScreeInPlayerController>(GetOwningPlayer());
	if (ScreeInPlayerController)
	{
		int32 ItemCount = ScreeInPlayerController->Inventory.Num();
		if (ItemCount <= 0)
		{
			CurrentIndex = -1;
		} else if (CurrentIndex < 0)
		{
			CurrentIndex = 0;
		} else if (CurrentIndex > ItemCount - 1)
		{
			CurrentIndex = ItemCount - 1;
		}
		
		for (int i = 0; i < ItemCount; ++i)
		{
			UPickupItemData* PickupItemData = ScreeInPlayerController->Inventory[i];
			UInventoryItemObject* InventoryItemObject = Cast<UInventoryItemObject>(InventoryItems->GetItemAt(i));
			InventoryItemObject->PickupItemData = PickupItemData;
			InventoryItemObject->bIsSelected = i == CurrentIndex;
		}

		for (int i = ItemCount; i < MaxItemCount; ++i)
		{
			UInventoryItemObject* InventoryItemObject = Cast<UInventoryItemObject>(InventoryItems->GetItemAt(i));
			InventoryItemObject->PickupItemData = nullptr;
			InventoryItemObject->bIsSelected = i == CurrentIndex;
		}

		InventoryItems->RegenerateAllEntries();
	}
}
