// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UBoxComponent;
class UPickupItemData;

UCLASS()
class SCREEIN_API ADoor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	UBoxComponent* DoorBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	TObjectPtr<UPickupItemData> Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bUnlocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bFakeDoor = false;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void LookAt_Implementation(const UObject* Interactor, const APlayerController* Controller, const UPrimitiveComponent* HitComponent, AActor*& LookAtActor, UPrimitiveComponent*& LookAtComponent, FString& Message) override;
	virtual void InteractWith_Implementation(const UObject* Interactor, const UPrimitiveComponent* HitComponent, bool& Interactable) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OpenDoor();

	UFUNCTION(BlueprintImplementableEvent)
	void KnockDoor();
};
