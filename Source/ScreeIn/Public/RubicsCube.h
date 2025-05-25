// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockPlane.h"
#include "GameFramework/Actor.h"
#include "RubicsCube.generated.h"

class ABlockPlane;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleSolved);

USTRUCT(BlueprintType)
struct FBlockPlaneData
{
	GENERATED_BODY()

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
	// ABlockPlane* BlockRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
	ECubeColor CurrentCubeColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
	ECubeColor CorrectCubeColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
	bool bCanInteract = false;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
	// bool bIsSelected = false;
};


UCLASS()
class SCREEIN_API ARubicsCube : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARubicsCube();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RubicsCube")
	FIntVector Size = FIntVector(1, 1, 1); // 默认 3×3×3 魔方

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "RubicsCube", meta = (AllowPrivateAccess = "true"))
	TArray<UChildActorComponent*> CubePlanes;

	UPROPERTY(EditAnywhere, Category = "Rubics Cube")
	TSubclassOf<AActor> BlockClass; // 可在蓝图中选择的方块对象

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Rubics Cube", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ABlockPlane>> Blocks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocks")
	TArray<FBlockPlaneData> BlocksProxy;

	UPROPERTY(BlueprintAssignable, Category = "Puzzle")
	FOnPuzzleSolved OnPuzzleSolved;
	
	UPROPERTY(EditAnywhere, Category = "Rubics Cube")
	float BlockSpacing = 5.0f; // 方块之间的间距

	UPROPERTY(EditAnywhere, Category = "Rubics Cube")
	UMaterialInterface* DefaultMaterial; // 默认材质

	// UPROPERTY(EditAnywhere, Category = "Rubics Cube")
	// TArray<ECubeColor> CubeColors; // 魔方的颜色

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rublics Cube", meta = (ExposeOnSpawn = "true"))
	bool bHasGenerated = false;


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void UnregisterAllComponents(bool bForReregister = false) override;

	// virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
	void GenerateRubicsCube();

	UFUNCTION(BlueprintCallable, Category = "RubicsCube")
	void SetMaterialForAllBlocks(UMaterialInterface* NewMaterial);

	UFUNCTION(BlueprintCallable, Category = "RubicsCube")
	bool CheckAllBlocksCorect();

	// 定义六个面的方向和位置
	TArray<FVector> PlaneDirections = {
		FVector(1, 0, 0),  // X 正方向（右）
		FVector(-1, 0, 0), // X 负方向（左）
		FVector(0, 1, 0),  // Y 正方向（前）
		FVector(0, -1, 0), // Y 负方向（后）
		FVector(0, 0, 1),  // Z 正方向（上）
		FVector(0, 0, -1)  // Z 负方向（下）
	};

	TArray<FRotator> PlaneRotations = {
		FRotator(0, -90, 90),   // X 正方向（右）：绕 Y 轴旋转 90 度
		FRotator(0, 90, 90),  // X 负方向（左）：绕 Y 轴旋转 -90 度
		FRotator(0, 0, 90),    // Y 正方向（前）：无需旋转
		FRotator(0, 180, 90),  // Y 负方向（后）：绕 Y 轴旋转 180 度
		FRotator(0, 0, 0),  // Z 正方向（上）：绕 X 轴旋转 -90 度
		FRotator(180, 0, 0)    // Z 负方向（下）：绕 X 轴旋转 90 度
	};
};







