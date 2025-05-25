// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RubikCubeBase.generated.h"

class ARubicsCube;

UCLASS()
class SCREEIN_API ARubikCubeBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARubikCubeBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RubikCube")
	AActor* RubicsCube;

	UPROPERTY(editAnywhere, BlueprintReadOnly, Category = "RubikCube")
	FRotator RotationSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
