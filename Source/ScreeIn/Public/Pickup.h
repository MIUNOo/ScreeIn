// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class UPickupItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickup);

UCLASS()
class SCREEIN_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	APickup();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	UPickupItemData* ItemData;

	UPROPERTY(BlueprintAssignable, Category = "Pickup")
	FOnPickup OnPickup;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void LookAt_Implementation(const UObject* Interactor, const APlayerController* Controller, const UPrimitiveComponent* HitComponent, AActor*& LookAtActor, UPrimitiveComponent*& LookAtComponent, FString& Message) override;
	virtual void InteractWith_Implementation(const UObject* Interactor, const UPrimitiveComponent* HitComponent, bool& Interactable) override;
};
