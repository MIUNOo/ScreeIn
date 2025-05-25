// Fill out your copyright notice in the Description page of Project Settings.


#include "Screen.h"

#include "ScreeInPlayerController.h"
#include "Camera/CameraComponent.h"


// Sets default values
AScreen::AScreen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenMesh"));
	RootComponent = ScreenMesh;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AScreen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScreen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AScreen::LookAt_Implementation(const UObject* Interactor, const APlayerController* Controller,
	const UPrimitiveComponent* HitComponent, AActor*& LookAtActor, UPrimitiveComponent*& LookAtComponent,
	FString& Message)
{
	LookAtActor = this;
	LookAtComponent = ScreenMesh;
	Message = "[LMB] to interact";
}

void AScreen::InteractWith_Implementation(const UObject* Interactor, const UPrimitiveComponent* HitComponent,
	bool& Interactable)
{
	IInteractionInterface::InteractWith_Implementation(Interactor, HitComponent, Interactable);
}

void AScreen::TriggerEnterScreen(AScreeInPlayerController* ScreeInPlayerController)
{
	if (!ScreeInPlayerController->bCanEnterScreen || !bCanEnterScreen)
	{
		return;
	}
	
	ScreeInPlayerController->SetViewTargetWithBlend(this, ScreenEnterDelay, VTBlend_EaseOut, 2.f);
}
