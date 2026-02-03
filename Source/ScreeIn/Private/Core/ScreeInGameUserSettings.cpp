// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ScreeInGameUserSettings.h"

void UScreeInGameUserSettings::SetMouseSensitivity(float Sensitivity)
{
	fMouseSensitivity = Sensitivity;
}

float UScreeInGameUserSettings::GetMouseSensitivity() const
{
	return fMouseSensitivity;
}

void UScreeInGameUserSettings::SetMotionBlur(bool bEnable)
{
	bEnableMotionBlur = bEnable;
}

bool UScreeInGameUserSettings::GetMotionBlur()
{
	return bEnableMotionBlur;
}

void UScreeInGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);

	if (IConsoleVariable* MotionBlurToggle =
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefaultFeature.MotionBlur")))
	{
		MotionBlurToggle->Set(bEnableMotionBlur ? 1 : 0, ECVF_SetByGameSetting);
	}
	
	
}
