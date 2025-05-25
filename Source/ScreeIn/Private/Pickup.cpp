// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "ScreeInCharacter.h"
#include "ScreeInPlayerController.h"


// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::LookAt_Implementation(const UObject* Interactor, const APlayerController* Controller,
	const UPrimitiveComponent* HitComponent, AActor*& LookAtActor, UPrimitiveComponent*& LookAtComponent,
	FString& Message)
{
	LookAtActor = this;
	LookAtComponent = MeshComponent;
	Message = "[LMB] Pickup";
}

void APickup::InteractWith_Implementation(const UObject* Interactor, const UPrimitiveComponent* HitComponent,
	bool& Interactable)
{
	if (const AScreeInCharacter* Character = Cast<AScreeInCharacter>(Interactor))
	{
		if (AScreeInPlayerController* PlayerController = Character->GetController<AScreeInPlayerController>())
		{
			PlayerController->AddItemToInventory(ItemData);
			OnPickup.Broadcast();
			Destroy();
		}
	}
}

