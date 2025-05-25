// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TelevisionWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTelevisionWidget::SetInfoText(const FText& Text)
{
	InfoText->SetText(Text);
	if (Text.IsEmpty())
	{
		BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		BackgroundImage->SetVisibility(ESlateVisibility::Visible);
	}
}
