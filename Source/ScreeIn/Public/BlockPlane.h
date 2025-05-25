// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
// #include "RubicsCube.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "BlockPlane.generated.h"

class UPickupItemData;
class ARubicsCube;

UENUM(BlueprintType)
enum class ECubeColor : uint8
{
	White UMETA(DisplayName = "White"),
	Yellow UMETA(DisplayName = "Yellow"),
	Blue UMETA(DisplayName = "Blue"),
	Red UMETA(DisplayName = "Red"),
	Green UMETA(DisplayName = "Green"),
	Orange UMETA(DisplayName = "Orange"),
	Grey UMETA(DisplayName = "Grey")
};


UCLASS()
class SCREEIN_API ABlockPlane : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABlockPlane();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* PlaneMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* OutlinePlaneMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (ExposeOnSpawn = "true"))
	UTextRenderComponent* Text;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (ExposeOnSpawn = "true"))
	ECubeColor CubeColor;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Color", meta = (ExposeOnSpawn = "true"))
	UMaterialInterface* MICMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (ExposeOnSpawn = "true"))
	bool bCanInteract = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (ExposeOnSpawn = "true"))
	bool bSwitchColor = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (ExposeOnSpawn = "true"))
	ARubicsCube* ParentRubicsCube;

	
	UFUNCTION(BlueprintCallable, Category = "Color")
	void ApplyCubeColor();
	void SetSharedMaterial(UMaterialInterface* NewMaterial);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Destroyed() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void LookAt_Implementation(const UObject* Interactor, const APlayerController* Controller, const UPrimitiveComponent* HitComponent, AActor*& LookAtActor, UPrimitiveComponent*& LookAtComponent, FString& Message) override;

	virtual void InteractWith_Implementation(const UObject* Interactor, const UPrimitiveComponent* HitComponent, bool& Interactable) override;

	static FLinearColor GetColorFromEnum(ECubeColor Color);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMaterial;


	
private:

	int32 CurrentColorIndex = 0;
	ECubeColor CubeColorArray[6] = {ECubeColor::White, ECubeColor::Yellow, ECubeColor::Blue, ECubeColor::Red, ECubeColor::Green, ECubeColor::Orange};
	UPickupItemData* SavedItemData = nullptr;
};
