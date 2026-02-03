// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "HAL/IConsoleManager.h"
#include "ScreeInGameUserSettings.generated.h"


/**
 * 
 */

UCLASS(BlueprintType,config=GameUserSettings)
class SCREEIN_API UScreeInGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
	public:
    
    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite)
    float fMouseSensitivity = 1.0f;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite)
	bool bEnableMotionBlur = false;

	UFUNCTION(BlueprintCallable)
	void SetMouseSensitivity(float Sensitivity);

	UFUNCTION(BlueprintPure)
	float GetMouseSensitivity() const;
	
	UFUNCTION(BlueprintCallable)
	void SetMotionBlur(bool bEnable);

	UFUNCTION(BlueprintPure)
	bool GetMotionBlur();

	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
};
