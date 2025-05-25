// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CrosshairWidget.h"

// UCrosshairWidget::UCrosshairWidget(const FObjectInitializer& ObjectInitializer)
// 	: Super(ObjectInitializer)
// {
// 	CanvasPanel = CreateDefaultSubobject<UCanvasPanel>(TEXT("CanvasPanel"));
// 	CrosshairWidget = CreateDefaultSubobject<UImage>(TEXT("CrosshairWidget"));
// 	Message = CreateDefaultSubobject<UTextBlock>(TEXT("Message"));
// }


void UCrosshairWidget::TriggerDisplayMessage(const FString& NewMessage)
{
	DisplayMessage(NewMessage);
}
