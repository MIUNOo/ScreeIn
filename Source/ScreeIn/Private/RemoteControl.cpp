// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoteControl.h"

#include "Television.h"


const int ARemoteControl::ButtonLayout[4][3] = {
	{ 1, 2, 3 },
	{ 4, 5, 6 },
	{ 7, 8, 9 },
	{ -1, 0, -1 }
};

// Sets default values
ARemoteControl::ARemoteControl()
	: CurrentHoveredNumber(0), CurrentHoveredX(0), CurrentHoveredY(0), CanHandleInput(true)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < 10; ++i)
	{
		NumberButtonUnlocked.Add(i, false);
	}
}

// Called when the game starts or when spawned
void ARemoteControl::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 10; ++i)
	{
		ToggleNumberVisible(i, NumberButtonUnlocked[i]);
	}

	for (TObjectPtr<UStaticMeshComponent> ButtonMesh : ControlButtonMeshes)
	{
		UMaterialInstanceDynamic* Material =
			ButtonMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, ButtonRegularMaterial);
		ControlButtonMaterials.Add(Material);
		ButtonMesh->SetMaterial(0, Material);
	}

	CurrentHoveredNumber = InitialHoveredNumber;
	CanHandleInput = true;
}

// Called every frame
void ARemoteControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARemoteControl::UnlockNumberButton(int Number)
{
	NumberButtonUnlocked[Number] = true;
	ToggleNumberVisible(Number, true);
}

void ARemoteControl::MoveHoveredNumber(int X, int Y)
{
	// int NewNumber = GetNumberWithDelta(CurrentHoveredNumber, X, Y);
	// if (NewNumber == CurrentHoveredNumber)
	// {
	// 	return;
	// }
	//
	// ControlButtonMeshes[CurrentHoveredNumber]->SetMaterial(0, ButtonRegularMaterial);
	// CurrentHoveredNumber = NewNumber;
	// ControlButtonMeshes[CurrentHoveredNumber]->SetMaterial(0, ControlButtonMaterials[CurrentHoveredNumber]);
}

void ARemoteControl::SelectHoveredNumber()
{
	// Television->EnterChannelNumber(CurrentHoveredNumber);
	// OnHoveredNumberSelected.Broadcast(CurrentHoveredNumber);
}

void ARemoteControl::ToggleHoveringVisible(bool bHoveringVisible)
{
	// if (bHoveringVisible)
	// {
	// 	ControlButtonMeshes[CurrentHoveredNumber]->SetMaterial(0, ControlButtonMaterials[CurrentHoveredNumber]);
	// }
	// else
	// {
	// 	ControlButtonMeshes[CurrentHoveredNumber]->SetMaterial(0, ButtonRegularMaterial);
	// }
}

void ARemoteControl::InputNumberKey(int Number)
{
	if (NumberButtonUnlocked[Number] && CanHandleInput)
	{
		// Television->EnterChannelNumber(Number);
		CanHandleInput = false;
		OnNumberInput.Broadcast(Number);
	}
}

void ARemoteControl::TurnTelevisionPower()
{
	Television->TurnPower();
}

void ARemoteControl::ToggleNumberVisible(int Number, bool bVisible)
{
	UStaticMeshComponent* ControlButtonMesh = ControlButtonMeshes[Number];
	ControlButtonMesh->SetHiddenInGame(!bVisible, true);
	ControlButtonMesh->SetCollisionEnabled(bVisible ? ECollisionEnabled::Type::QueryAndPhysics : ECollisionEnabled::Type::NoCollision);
	
	USceneComponent* EmptyButtonComponent = EmptyButtonComponents[Number];
	EmptyButtonComponent->SetHiddenInGame(bVisible, true);
}

int ARemoteControl::GetNumberWithDelta(int CurrentNumber, int DeltaX, int DeltaY)
{
	int NewX = CurrentHoveredX + DeltaX;
	int NewY = CurrentHoveredY + DeltaY;

	NewX = FMath::Clamp(NewX, 0, 2);
	NewY = FMath::Clamp(NewY, 0, 3);

	int NewNumber = ButtonLayout[NewY][NewX];
	if (NewNumber == -1 || !NumberButtonUnlocked[NewNumber])
	{
		return CurrentNumber;
	}

	CurrentHoveredX = NewX;
	CurrentHoveredY = NewY;
	return NewNumber;
}
