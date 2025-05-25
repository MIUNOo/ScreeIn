// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Door.h"

#include "ScreeInCharacter.h"
#include "ScreeInPlayerController.h"
#include "Components/BoxComponent.h"


// Sets default values
ADoor::ADoor()
{
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	DoorBlock = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorBlock"));
	DoorBlock->SetupAttachment(DoorMesh);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::LookAt_Implementation(const UObject* Interactor, const APlayerController* Controller,
	const UPrimitiveComponent* HitComponent, AActor*& LookAtActor, UPrimitiveComponent*& LookAtComponent,
	FString& Message)
{
	LookAtActor = this;
	LookAtComponent = DoorMesh;

	AScreeInPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<AScreeInPlayerController>();

	UPickupItemData* SelectedItem = PlayerController->GetSelectedInventoryItem();
	if (SelectedItem && SelectedItem == Key || bUnlocked)
	{
		Message = "[LMB] Open";
	} else
	{
		Message = "Knock";
	}
}

void ADoor::InteractWith_Implementation(const UObject* Interactor, const UPrimitiveComponent* HitComponent,
	bool& Interactable)
{
	if (const AScreeInCharacter* Character = Cast<AScreeInCharacter>(Interactor))
	{
		if (AScreeInPlayerController* PlayerController = Character->GetController<AScreeInPlayerController>())
		{
			if (bUnlocked)
			{
				OpenDoor();
				DoorBlock->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				return;
			}
			
			UPickupItemData* SelectedItem = PlayerController->GetSelectedInventoryItem();
			if (SelectedItem && SelectedItem == Key)
			{
				OpenDoor();
				DoorBlock->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				PlayerController->RemoveItemFromInventory(SelectedItem);
			}
			else
			{
				KnockDoor();
			}
		}
	}
}

