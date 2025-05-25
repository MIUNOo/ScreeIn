#include "ExitTrigger.h"

// Sets default values
AExitTrigger::AExitTrigger()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	
	ExitBoard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExitBoard"));
	ExitBoard->SetupAttachment(RootComponent);
	// RootComponent = ExitBoard;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AExitTrigger::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AExitTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExitTrigger::LookAt_Implementation(const UObject* Interactor, const APlayerController* Controller,
	const UPrimitiveComponent* HitComponent, AActor*& LookAtActor, UPrimitiveComponent*& LookAtComponent,
	FString& Message)
{
	LookAtActor = this;
	LookAtComponent = ExitBoard;
	Message = "Hold Right Mouse Button Exit";
}

void AExitTrigger::InteractWith_Implementation(const UObject* Interactor, const UPrimitiveComponent* HitComponent,
                                               bool& Interactable)
{
	Interactable = true;
}
