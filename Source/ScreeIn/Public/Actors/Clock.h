// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Clock.generated.h"

class UAkAudioEvent;
class UAkComponent;

UCLASS()
class SCREEIN_API AClock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClock();

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Clock")
	UStaticMeshComponent* ClockMesh;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Clock")
	UStaticMeshComponent* HourHand;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Clock")
	UStaticMeshComponent* MinuteHand;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Clock")
	UStaticMeshComponent* SecondHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clock")
	UAkComponent* AkAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clock")
	USoundBase* TickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clock")
	UAkAudioEvent* TickSoundAk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clock")
	bool bPlayTickSound = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void TickSecondHand();

	UFUNCTION(BlueprintCallable)
	void TogglePlayTickSound(bool bPlay);

private:
	FTimerHandle SecondHandTimer;
};
