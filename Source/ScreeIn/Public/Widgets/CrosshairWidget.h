// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CrosshairWidget.generated.h"


UCLASS()
class SCREEIN_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crosshair")
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crosshair")
	UImage* CrosshairWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crosshair")
	UTextBlock* Message;

	UFUNCTION(BlueprintImplementableEvent, Category = "Crosshair")
	void DisplayMessage(const FString& NewMessage);

	UFUNCTION(BlueprintCallable, Category = "Crosshair")
	void TriggerDisplayMessage(const FString& NewMessage);
	
};
