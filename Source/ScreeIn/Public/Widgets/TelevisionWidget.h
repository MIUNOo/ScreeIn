// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TelevisionWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SCREEIN_API UTelevisionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television", meta = (BindWidget))
	UTextBlock* InfoText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television", meta = (BindWidget))
	UImage* BackgroundImage;

	UFUNCTION(BlueprintCallable, Category = "Television")
	void SetInfoText(const FText& Text);
};
