// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScreeInGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AScreeInGameMode::AScreeInGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

AActor* AScreeInGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> FoundStarts;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, APlayerStart::StaticClass(), FName("Init"), FoundStarts);

	if (FoundStarts.Num() > 0)
	{
		return FoundStarts[0];
	}

	// fallback
	return Super::ChoosePlayerStart_Implementation(Player);
}
