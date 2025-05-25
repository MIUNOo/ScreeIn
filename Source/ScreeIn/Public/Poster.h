// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Poster.generated.h"

UCLASS()
class SCREEIN_API APoster : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APoster();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Poster")
	FFilePath Path;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Poster")
	UTexture2D* DefaultTexture;

	// Function to load a PNG image from the specified file path and convert it to a texture
	UFUNCTION(BlueprintCallable, Category = "Custom")
	UTexture2D* LoadTextureFromPath(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void ChangeMaterialFromPath(const FString& FilePath);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* MaterialTemplate; // Material Template pointer, initialized in child

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
