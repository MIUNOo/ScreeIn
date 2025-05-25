// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockPlane.h"

#include "PickupItemData.h"
#include "RubicsCube.h"
#include "ScreeInPlayerController.h"


// #include <GL/glext.h>


// Sets default values
ABlockPlane::ABlockPlane()
{

	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh"));
	RootComponent = PlaneMesh;

	OutlinePlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OutlinePlaneMesh"));
	OutlinePlaneMesh->SetupAttachment(PlaneMesh);

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	Text->SetupAttachment(PlaneMesh);
	Text->SetHorizontalAlignment(EHTA_Center);
	Text->SetVerticalAlignment(EVRTA_TextCenter);
	Text->SetTextRenderColor(FColor::Black);
	Text->SetText(FText::FromString(""));
	

	OutlinePlaneMesh->SetRelativeScale3D(FVector(1.1875f, 1.1875f, 1.0f));

	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABlockPlane::ApplyCubeColor()
{
	
	if (!PlaneMesh) return;


	if (MICMaterial)
	{
		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(MICMaterial, this);
			if (DynamicMaterial)
			{
				PlaneMesh->SetMaterial(0, DynamicMaterial);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create DynamicMaterial!"));
			}
		}

		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", GetColorFromEnum(CubeColor));
		}	
		
	}




}

void ABlockPlane::SetSharedMaterial(UMaterialInterface* NewMaterial)
{
	if (NewMaterial)
	{
		UMaterialInterface* DefaultMaterial = NewMaterial;
		PlaneMesh->SetMaterial(0, DefaultMaterial);
	}
}

// Called when the game starts or when spawned
void ABlockPlane::BeginPlay()
{
	Super::BeginPlay();

	// ApplyCubeColor();
	
}


void ABlockPlane::OnConstruction(const FTransform& Transform)
{
	// Super::OnConstruction(Transform);
	//NEEDFIX
	ApplyCubeColor();
}

void ABlockPlane::Destroyed()
{
	Super::Destroyed();
	// UE_LOG(LogTemp, Error, TEXT("BlockPlane %s is destroyed!"), *GetName());
}

// Called every frame
void ABlockPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlockPlane::LookAt_Implementation(const UObject* Interactor, const APlayerController* Controller,
	const UPrimitiveComponent* HitComponent, AActor*& LookAtActor, UPrimitiveComponent*& LookAtComponent,
	FString& Message)
{
	// IInteractionInterface::LookAt_Implementation(Interactor, Controller, HitComponent, LookAtActor, LookAtComponent,Message);
	LookAtActor = this;
	LookAtComponent = PlaneMesh;
	if (bCanInteract && CubeColor == ECubeColor::Grey)
	{
		Message = "[LMB] Place holding piece";
	}
	else
	{
		Message = "";
	}

	
}

void ABlockPlane::InteractWith_Implementation(const UObject* Interactor, const UPrimitiveComponent* HitComponent,
	bool& Interactable)
{
	// IInteractionInterface::InteractWith_Implementation(Interactor, HitComponent, Interactable);

	if (!bCanInteract)
	{
		return;
	}

	AScreeInPlayerController* PlayerController = Cast<AScreeInPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController) 
	{
		UPickupItemData* ItemToAdd = nullptr;
		if (CubeColor != ECubeColor::Grey)
		{
			ECubeColor TempColor = CubeColor;
			ItemToAdd = NewObject<UPickupItemData>();
			ItemToAdd->CubeColor = TempColor;
			ItemToAdd->ItemIcon = SavedItemData->ItemIcon;
			FString ColorName = UEnum::GetDisplayValueAsText(TempColor).ToString();
			ItemToAdd->ItemName = FText::FromString(FString::Printf(TEXT("%s Cube Piece"), *ColorName));
			ItemToAdd->ItemType = EPickupItemType::BlockPlane;
			
			CubeColor = ECubeColor::Grey;
		}

		UObject* Item = PlayerController->GetSelectedInventoryItem();
		if (Item)
		{
			UPickupItemData* PickupData = Cast<UPickupItemData>(Item);
			if (PickupData->ItemType == EPickupItemType::BlockPlane)
			{
				ECubeColor SelectedColor = PickupData->CubeColor;
				
				// Switch Color
				// ECubeColor TempColor = CubeColor;
				CubeColor = SelectedColor;
				SavedItemData = PickupData;
				PlayerController->RemoveItemFromInventory(PickupData);
				// UPickupItemData* DataFromCube = NewObject<UPickupItemData>();
				// DataFromCube->CubeColor = TempColor;
				// FString ColorName = UEnum::GetDisplayValueAsText(TempColor).ToString();
				// DataFromCube->ItemName = FText::FromString(FString::Printf(TEXT("%s Cube Piece"), *ColorName));
			}
		}

		ApplyCubeColor();

		if (ItemToAdd)
		{
			PlayerController->AddItemToInventory(ItemToAdd);
		}
		
		// if (Item) 
		// {
		// 	UPickupItemData* PickupData = Cast<UPickupItemData>(Item);
		// 	if (PickupData) 
		// 	{
		// 		if (PickupData->bIsNumberButton)
		// 		{
		// 			return;
		// 		}
		// 		
		// 		ECubeColor SelectedColor = PickupData->CubeColor;
		// 		if (CubeColor == ECubeColor::Grey)
		// 		{
		// 			CubeColor = SelectedColor;
		// 			ApplyCubeColor();
		// 			PlayerController->RemoveItemFromInventory(PickupData);
		// 		}
		// 		else
		// 		{
		// 			
		//
		// 			ApplyCubeColor();
		//
		// 		}
		//
		// 	}
		// }


	}
	// else if (bSwitchColor)
	// {
	// 	CurrentColorIndex = (CurrentColorIndex + 1) % 6;  // 顺序循环切换颜色
	// 	CubeColor = CubeColorArray[CurrentColorIndex];     // 设置新的颜色
	// 	ApplyCubeColor();  // 应用新的颜色
	// }
	

	if (ParentRubicsCube)
	{
		if (ParentRubicsCube->CheckAllBlocksCorect())
		{
			UE_LOG(LogTemp, Error, TEXT("All blocks are correct!"));
		}
	}

}

FLinearColor ABlockPlane::GetColorFromEnum(ECubeColor Color)
{
	switch (Color)
	{
	case ECubeColor::White: return FLinearColor::White;
	case ECubeColor::Yellow: return FLinearColor(1.0f, 1.0f, 0.0f);
	case ECubeColor::Blue: return FLinearColor(0.0f, 0.0f, 1.0f);
	case ECubeColor::Red: return FLinearColor(1.0f, 0.0f, 0.0f);
	case ECubeColor::Green: return FLinearColor(0.0f, 1.0f, 0.0f);
	case ECubeColor::Orange: return FLinearColor(1.0f, 0.65f, 0.0f);
	case ECubeColor::Grey: return FLinearColor::Black;
	default: return FLinearColor::Gray;
	}
}




