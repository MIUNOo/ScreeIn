// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "RemoteControl.generated.h"

class URemoteControlButtonComponent;
class ATelevision;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoveredNumberSelected, int, Number);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNumberInput, int, Number);

UCLASS()
class SCREEIN_API ARemoteControl : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARemoteControl();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RemoteControl")
	TArray<TObjectPtr<UStaticMeshComponent>> ControlButtonMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RemoteControl")
	TArray<TObjectPtr<UMaterialInstanceDynamic>> ControlButtonMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RemoteControl")
	TArray<TObjectPtr<USceneComponent>> EmptyButtonComponents;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "RemoteControl")
	TObjectPtr<ATelevision> Television;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RemoteControl")
	TMap<int, bool> NumberButtonUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RemoteControl")
	TObjectPtr<UMaterialInstance> ButtonRegularMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RemoteControl")
	TObjectPtr<UMaterialInstance> ButtonHoveredMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RemoteControl")
	int32 InitialHoveredNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "RemoteControl")
	FOnHoveredNumberSelected OnHoveredNumberSelected;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "RemoteControl")
	FOnNumberInput OnNumberInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RemoteControl")
	bool CanHandleInput;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "RemoteControl")
	void UnlockNumberButton(int Number);

	UFUNCTION(BlueprintCallable, Category = "RemoteControl")
	void MoveHoveredNumber(int X, int Y);

	UFUNCTION(BlueprintCallable, Category = "RemoteControl")
	void SelectHoveredNumber();

	UFUNCTION(BlueprintCallable, Category = "RemoteControl")
	void ToggleHoveringVisible(bool bHoveringVisible);

	UFUNCTION(BlueprintCallable, Category = "RemoteControl")
	void InputNumberKey(int Number);

	UFUNCTION(BlueprintCallable, Category = "RemoteControl")
	void TurnTelevisionPower();

private:
	void ToggleNumberVisible(int Number, bool bVisible);
	int GetNumberWithDelta(int CurrentNumber, int DeltaX, int DeltaY);
	
	int CurrentHoveredNumber;
	int CurrentHoveredX;
	int CurrentHoveredY;

	static const int ButtonLayout[4][3];
};
