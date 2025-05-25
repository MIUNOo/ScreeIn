// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ScreeInGameMode.generated.h"

UCLASS(minimalapi)
class AScreeInGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AScreeInGameMode();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};



