// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryItem.h"

#include "PickupItemData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Widgets/InventoryItemObject.h"

void UInventoryItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UInventoryItemObject* ItemData = Cast<UInventoryItemObject>(ListItemObject);
	if (ItemData->PickupItemData)
	{
		// ItemText->SetText(ItemData->PickupItemData->ItemName);
		UTexture2D* ItemTexture = ItemData->PickupItemData->ItemIcon;
		// FSlateBrush Brush;
		// Brush.SetResourceObject(ItemTexture);
		// Brush.ImageSize = FVector2D(ItemTexture->GetSizeX(), ItemTexture->GetSizeY());
		ItemIcon->SetVisibility(ESlateVisibility::Visible);
		ItemIcon->SetBrushFromTexture(ItemTexture);
	}
	else
	{
		// ItemText->SetText(FText::GetEmpty());
		ItemIcon->SetBrush(FSlateBrush());
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		// ItemIcon->SetBrushFromTexture(nullptr);
	}

	if (ItemData->bIsSelected)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Visible);
	} 
	else
	{
		SelectedImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
