// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorGenerator.generated.h"

class ADoor;

UCLASS()
class SCREEIN_API ADoorGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoorGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	TSubclassOf<AActor> DoorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	int32 MinGroupCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	int32 MaxGroupCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	float GroupInterval = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	int32 TotalCount = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	float RandomRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	bool RandomRotation = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Door")
	void StartGeneration();

	UFUNCTION()
	void GenerateGroup();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorGenerated(AActor* Door);

private:
	int32 CurrentCount = 0;
	FTimerHandle GenerationTimerHandle;
};
