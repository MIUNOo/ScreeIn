// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ScreeInGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SCREEIN_API UScreeInGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
};
