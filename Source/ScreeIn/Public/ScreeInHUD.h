// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "Components/Widget.h"
#include "GameFramework/HUD.h"
#include "ScreeInHUD.generated.h"

/**
 * 
 */
UCLASS()
class SCREEIN_API AScreeInHUD : public AHUD
{
	GENERATED_BODY()

public:

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	// UWidget* CrosshairWidget;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void DisplayMessage(const FString& NewMessage);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD")
	void ToggleHud();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ChangeCrosshairAlpha(float Alpha);
};
