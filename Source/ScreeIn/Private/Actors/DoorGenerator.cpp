// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DoorGenerator.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ADoorGenerator::ADoorGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADoorGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorGenerator::StartGeneration()
{
	CurrentCount = 0;
	GetWorld()->GetTimerManager().SetTimer(
		GenerationTimerHandle,
		this,
		&ADoorGenerator::GenerateGroup,
		GroupInterval,
		true);
}

void ADoorGenerator::GenerateGroup()
{
	int32 Count = FMath::RandRange(MinGroupCount, MaxGroupCount);
	if (CurrentCount + Count > TotalCount)
	{
		Count = TotalCount - CurrentCount;
		GetWorld()->GetTimerManager().ClearTimer(GenerationTimerHandle);
	}
	
	for (int32 i = 0; i < Count; ++i)
	{
		int32 RetryCount = 0;
		while (RetryCount < 10)
		{
			FVector RandVec = FMath::VRand();
			RandVec.Z = 0.f;
			RandVec.Normalize();
			FVector location = GetActorLocation() + RandVec * FMath::RandRange(-RandomRadius, RandomRadius);
			FRotator rotation = FRotator::ZeroRotator;
			rotation.Yaw = FMath::RandRange(0.f, 360.f);
			if (RandomRotation)
			{
				rotation.Pitch = FMath::RandRange(-180.f, 180.f);
				rotation.Roll = FMath::RandRange(-180.f, 180.f);
			}
			
			FTransform transform(rotation, location);

			FVector Start = location + FVector(0.f, 0.f, 150.f); // Start above the ground
			FVector End = location + FVector(0.f, 0.f, 150.f);

			FVector HalfSize(100.f, 100.f, 100.f); // Box half-extent (so full size is 100x100x100)
			FRotator Orientation = rotation;

			FHitResult HitResult;

			bool bHit = UKismetSystemLibrary::BoxTraceSingle(
				this,                            // World context
				Start,
				End,
				HalfSize,
				Orientation,
				ETraceTypeQuery::TraceTypeQuery1, // Use appropriate trace channel
				false,                          // Trace complex
				TArray<AActor*>(),             // Actors to ignore
				EDrawDebugTrace::ForDuration,  // Optional: draw debug box
				HitResult,
				true                           // Ignore self
			);

			if (bHit)
			{
				++RetryCount;
				continue;
			}

			ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(this, FName("ParkinglotMap"));
			if (StreamingLevel && StreamingLevel->IsLevelLoaded())
			{
				ULevel* Level = StreamingLevel->GetLoadedLevel();
				if (Level)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.OverrideLevel = Level;

					AActor* Door = GetWorld()->SpawnActor<AActor>(DoorClass, transform, SpawnParams);
					OnDoorGenerated(Door);
				}
			}
			
			++CurrentCount;
			break;
		}
		
	}
}

