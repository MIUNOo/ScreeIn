// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RubikCubeBase.h"

#include "RubicsCube.h"


// Sets default values
ARubikCubeBase::ARubikCubeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARubikCubeBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARubikCubeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RubicsCube)
	{
		FQuat Quat = RubicsCube->GetActorQuat();
		FQuat RotateQuat = FQuat(RotationSpeed * DeltaTime);
		FQuat NewQuat = RotateQuat * Quat;
		FRotator NewRotation = NewQuat.Rotator();
		RubicsCube->SetActorRotation(NewRotation);
	}
}

