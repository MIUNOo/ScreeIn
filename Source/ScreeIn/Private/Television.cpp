// Fill out your copyright notice in the Description page of Project Settings.


#include "Television.h"

#include "PickupItemData.h"
#include "RemoteControl.h"
#include "ScreeInPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Widgets/TelevisionWidget.h"


// Sets default values
ATelevision::ATelevision()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TelevisionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TelevisionWidget"));
	TelevisionWidget->SetupAttachment(RootComponent);
	
	SeatViewTarget = nullptr;
}

// Called when the game starts or when spawned
void ATelevision::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = TelevisionWidget->GetWidget();
	TelevisionWidgetInstance = Cast<UTelevisionWidget>(Widget);
	TelevisionWidgetInstance->SetInfoText(FText::FromString(""));

	for (FTVChannel& Channel : Channels)
	{
		for (auto ChannelNumber : Channel.ChannelNumbers)
		{
			ChannelMap.Add(ChannelNumber, &Channel);
		}
	}

	if (bInitialPowerOn)
	{
		TurnPowerOn();
	}
}

// Called every frame
void ATelevision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATelevision::TriggerEnterScreen(AScreeInPlayerController* ScreeInPlayerController)
{
	Super::TriggerEnterScreen(ScreeInPlayerController);
}

AScreeInCharacter* ATelevision::GetSwitchToCharacter()
{
	FTVChannel Channel = GetChannel(CurrentChannel);
	return Channel.Character.Get();
}

FTVChannel& ATelevision::GetChannel(int ChannelNumber)
{
	if (ChannelMap.Contains(ChannelNumber))
	{
		return *ChannelMap[ChannelNumber];
	}

	return Channels[0];
}

void ATelevision::SwitchChannelWithDelay(int Channel)
{
	if (!bIsPowerOn) return;
	
	if (ChannelMap.Contains(Channel))
	{
		bCanEnterScreen = false;
		OnShowStaticNoise.Broadcast();

		GetWorld()->GetTimerManager().SetTimer(
			SwitchChannelTimerHandle,
			[this, Channel]()
			{
				SwitchChannel(Channel);
			},
			ChannelSwitchDelay,
			false);
	}
}

void ATelevision::SwitchChannel(int Channel)
{
	if (!bIsPowerOn) return;
	
	bCanEnterScreen = true;
	CurrentChannel = Channel;
	OnSwitchChannel.Broadcast(CurrentChannel);
}

void ATelevision::EnterChannelNumber(int Number)
{
	if (!bIsPowerOn) return;

	bCanEnterScreen = false;
	if (EnterChannelNumberTimerHandle.IsValid())
	{
		if (CurrentChannelNumber < 0)
		{
			return;
		}

		int NewChannelNumber = CurrentChannelNumber * 10 + Number;
		if (NewChannelNumber < MaxChannelNumber)
		{
			CurrentChannelNumber = NewChannelNumber;
		}
	}
	else
	{
		CurrentChannelNumber = Number;
	}
	
	TelevisionWidgetInstance->SetInfoText(FText::FromString(FString::FromInt(CurrentChannelNumber)));

	RestartChannelNumberEnterTimer();
}

void ATelevision::TurnPower()
{
	bIsPowerOn = !bIsPowerOn;
	if (EnterChannelNumberTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(EnterChannelNumberTimerHandle);
	}
	CurrentChannelNumber = 0;
	TelevisionWidgetInstance->SetInfoText(FText::FromString(""));

	if (bIsPowerOn)
	{
		bCanEnterScreen = false;
		OnShowStaticNoise.Broadcast();
	}

	OnPowerTurned.Broadcast(bIsPowerOn);
}

void ATelevision::TurnPowerOn()
{
	bIsPowerOn = true;
	if (EnterChannelNumberTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(EnterChannelNumberTimerHandle);
	}
	CurrentChannelNumber = 0;
	TelevisionWidgetInstance->SetInfoText(FText::FromString(""));

	bCanEnterScreen = false;
	OnShowStaticNoise.Broadcast();
	
	OnPowerTurned.Broadcast(bIsPowerOn);
}

void ATelevision::RestartChannelNumberEnterTimer()
{
	if (EnterChannelNumberTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(EnterChannelNumberTimerHandle);
	}

	GetWorld()->GetTimerManager().SetTimer(
		EnterChannelNumberTimerHandle,
		[this]()
		{
			bCanEnterScreen = true;
			TelevisionWidgetInstance->SetInfoText(FText::FromString(""));
			if (CurrentChannelNumber < 0)
			{
				CurrentChannelNumber = 0;
				return;
			}
			
			SwitchChannelWithDelay(CurrentChannelNumber);
			CurrentChannelNumber = 0;
		},
		ChannelNumberEnterWaitTime,
		false);
}

void ATelevision::SwitchToTelevisionView(AScreeInPlayerController* ScreeInPlayerController)
{
	// bCanEnterScreen = false;
	// ScreeInPlayerController->SetViewTargetWithBlend(SeatViewTarget, SeatViewTransitionDelay, VTBlend_EaseOut, 2.f);
	// GetWorld()->GetTimerManager().SetTimer(
	// 	SeatViewTransitionTimerHandle,
	// 	[this, ScreeInPlayerController]()
	// 	{
	// 		OnFinishSeatViewTransition(ScreeInPlayerController);
	// 	},
	// 	SeatViewTransitionDelay,
	// 	false);
}

void ATelevision::OnFinishSeatViewTransition(AScreeInPlayerController* ScreeInPlayerController)
{
	// bCanEnterScreen = true;
	AddNumberButtonFromInventory(ScreeInPlayerController);
}

void ATelevision::AddNumberButtonFromInventory(AScreeInPlayerController* ScreeInPlayerController)
{
	TArray<TObjectPtr<UPickupItemData>> Inventory = ScreeInPlayerController->Inventory;
	TArray<TObjectPtr<UPickupItemData>> ItemsToRemove;
	bool bNewButtonAdded = false;
	for (UPickupItemData* ItemData : Inventory)
	{
		if (ItemData->ItemType == EPickupItemType::NumberButton)
		{
			RemoteControl->UnlockNumberButton(ItemData->Number);
			ItemsToRemove.Add(ItemData);
			bNewButtonAdded = true;
		}
	}

	for (UPickupItemData* ItemData : ItemsToRemove)
	{
		ScreeInPlayerController->RemoveItemFromInventory(ItemData);
	}

	if (bNewButtonAdded)
	{
		OnNewButtonAdded.Broadcast();
	}
}

void ATelevision::TogglePostProcessVolume(bool bEnable)
{
	PostProcessVolume->bEnabled = bEnable;
}

