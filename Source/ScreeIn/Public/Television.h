// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "Screen.h"
#include "Television.generated.h"

class UTelevisionWidget;
class UWidgetComponent;
class ARemoteControl;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchChannel, int, Channel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowStaticNoise);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewButtonAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPowerTurned, bool, bIsPowerOn);

USTRUCT(BlueprintType)
struct FTVChannel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television")
	TObjectPtr<AScreeInCharacter> Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television")
	TObjectPtr<UMaterialInterface> ScreenMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television")
	TArray<int32> ChannelNumbers;
};

UCLASS()
class SCREEIN_API ATelevision : public AScreen
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATelevision();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television")
	UWidgetComponent* TelevisionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television")
	TArray<FTVChannel> Channels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television")
	float ChannelSwitchDelay = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television")
	float SeatViewTransitionDelay = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television")
	float ChannelNumberEnterWaitTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television")
	int MaxChannelNumber = 99;

	UPROPERTY(BlueprintAssignable, Category = "Television")
	FOnSwitchChannel OnSwitchChannel;

	UPROPERTY(BlueprintAssignable, Category = "Television")
	FOnShowStaticNoise OnShowStaticNoise;

	UPROPERTY(BlueprintAssignable, Category = "Television")
	FOnPowerTurned OnPowerTurned;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television")
	AActor* SeatViewTarget;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Television")
	TObjectPtr<ARemoteControl> RemoteControl;

	UPROPERTY(BlueprintAssignable, Category = "Television")
	FOnNewButtonAdded OnNewButtonAdded;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Television")
	APostProcessVolume* PostProcessVolume;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Television")
	bool bInitialPowerOn = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TriggerEnterScreen(AScreeInPlayerController* ScreeInPlayerController) override;
	virtual AScreeInCharacter* GetSwitchToCharacter() override;

	UFUNCTION(BlueprintCallable, Category = "Television")
	FTVChannel& GetChannel(int ChannelNumber);

	void SwitchChannelWithDelay(int Channel);

	UFUNCTION(BlueprintCallable, Category = "Television")
	void SwitchChannel(int Channel);

	UFUNCTION(BlueprintCallable, Category = "Television")
	void EnterChannelNumber(int Number);

	UFUNCTION(BlueprintCallable, Category = "Television")
	void TurnPower();

	UFUNCTION(BlueprintCallable, Category = "Television")
	void TurnPowerOn();
	
	void RestartChannelNumberEnterTimer();

	void SwitchToTelevisionView(AScreeInPlayerController* ScreeInPlayerController);
	void OnFinishSeatViewTransition(AScreeInPlayerController* ScreeInPlayerController);

	void AddNumberButtonFromInventory(AScreeInPlayerController* ScreeInPlayerController);

	void TogglePostProcessVolume(bool bEnable);

	TMap<int32, FTVChannel*> ChannelMap;
	TObjectPtr<UTelevisionWidget> TelevisionWidgetInstance;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Television")
	int CurrentChannel = -1;
	
	int CurrentChannelNumber = 0;
	FTimerHandle SwitchChannelTimerHandle;
	FTimerHandle SeatViewTransitionTimerHandle;
	FTimerHandle EnterChannelNumberTimerHandle;
	bool bIsPowerOn = false;
};
