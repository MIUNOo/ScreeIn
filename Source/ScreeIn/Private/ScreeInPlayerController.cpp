// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreeInPlayerController.h"

#include "EnhancedInputComponent.h"
#include "ScreeInHUD.h"
#include "Screen.h"
#include "Television.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "ScreeIn/Public/ScreeInCharacter.h"

void AScreeInPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport(10);

			// SetPause(true);

			// Enable UI-only input
			SetShowMouseCursor(true);

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);
		}
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, APlayerStart::StaticClass(), FName("SecondMainRoom"), FoundActors);
	if (FoundActors.Num() > 0)
	{
		SecondMainRoomPlayerStart = Cast<APlayerStart>(FoundActors[0]);
	}

	InventoryIndex = 0;
}

void AScreeInPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (PauseAction)
		{
			EnhancedInput->BindAction(PauseAction, ETriggerEvent::Started, this, &AScreeInPlayerController::TogglePauseMenu);
		}
	}
}

void AScreeInPlayerController::EnterTelevision(ATelevision* Television)
{
	if (bIsTransitioning || !Television->bCanEnterScreen || !Television->bIsPowerOn) return;

	bIsTransitioning = true;
	AScreeInCharacter* CurrentCharacter = GetPawn<AScreeInCharacter>();
	AScreeInCharacter* NewCharacter = Television->GetSwitchToCharacter();

	SetIgnoreLookInput(true);
	CurrentCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	CurrentCharacter->GetCharacterMovement()->DisableMovement();
	GetPawn<AScreeInCharacter>()->StoreLastRotation(GetControlRotation());
	
	Television->TogglePostProcessVolume(true);
	SetViewTargetWithBlend(Television, Television->ScreenEnterDelay, VTBlend_EaseIn, 2.f);
	OnStartEnterScreen.Broadcast();
	GetWorldTimerManager().SetTimer(
		EnterScreenTimerHandle,
		[this, NewCharacter, Television]()
		{
			EnterScreen(NewCharacter, Television);
			bIsTransitioning = false;
		},
		Television->ScreenEnterDelay,
		false);
}

void AScreeInPlayerController::HideMainMenu()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}

	SetPause(false);

	// Switch to Game Only input
	SetShowMouseCursor(false);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	GetHUD<AScreeInHUD>()->ToggleHud();

	bGameStarted = true;
}

void AScreeInPlayerController::TogglePauseMenu()
{
	if (!bGameStarted)
	{
		return;
	}
	
	if (!bIsPauseMenuVisible)
	{
		if (PauseMenuWidgetClass)
		{
			PauseMenuWidget = CreateWidget<UUserWidget>(this, PauseMenuWidgetClass);
			if (PauseMenuWidget)
			{
				PauseMenuWidget->AddToViewport(10);
				SetPause(true);
				SetShowMouseCursor(true);

				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				SetInputMode(InputMode);
			}
		}
	} 
	else
	{
		if (PauseMenuWidget)
		{
			PauseMenuWidget->RemoveFromParent();
			PauseMenuWidget = nullptr;
		}

		SetPause(false);
		SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}

	bIsPauseMenuVisible = !bIsPauseMenuVisible;
}

void AScreeInPlayerController::EnterScreen(AScreeInCharacter* NewCharacter, AScreen* Screen)
{
	AScreeInCharacter* CurrentCharacter = GetPawn<AScreeInCharacter>();
	
	// loop enter screen
	if (CurrentCharacter == NewCharacter)
	{
		SetViewTarget(NewCharacter);
		NewCharacter->CurrentTelevision = nullptr;
		NewCharacter->CurrentRemoteControl = nullptr;
		SetIgnoreLookInput(false);
	}
	else
	{
		ControlledCharactersStack.Push(CurrentCharacter);
		ScreenStack.Push(Screen);

		Possess(NewCharacter);
	}
	
	NewCharacter->ToggleVisibleInGame(true);
	SetControlRotation(NewCharacter->LastRotation);
	SetIgnoreLookInput(false);

	if (ATelevision* Television = Cast<ATelevision>(Screen))
	{
		OnEnterScreen.Broadcast(Television->CurrentChannel);
	}

	AScreeInHUD* HUD = GetHUD<AScreeInHUD>();
	if (HUD)
	{
		HUD->ToggleHud();
	}
}

void AScreeInPlayerController::EnterScreen(AScreen* Screen)
{
	if (bIsTransitioning || !Screen->bCanEnterScreen) return;

	bIsTransitioning = true;
	AScreeInCharacter* CurrentCharacter = GetPawn<AScreeInCharacter>();
	AScreeInCharacter* NewCharacter = Screen->GetSwitchToCharacter();

	SetIgnoreLookInput(true);
	CurrentCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	CurrentCharacter->GetCharacterMovement()->DisableMovement();
	GetPawn<AScreeInCharacter>()->StoreLastRotation(GetControlRotation());
	
	// Television->TogglePostProcessVolume(true);
	SetViewTargetWithBlend(Screen, Screen->ScreenEnterDelay, VTBlend_EaseIn, 2.f);
	OnStartEnterScreen.Broadcast();
	GetWorldTimerManager().SetTimer(
		EnterScreenTimerHandle,
		[this, NewCharacter, Screen]()
		{
			EnterScreen(NewCharacter, Screen);
			bIsTransitioning = false;
		},
		Screen->ScreenEnterDelay,
		false);

	AScreeInHUD* HUD = GetHUD<AScreeInHUD>();
	if (HUD)
	{
		HUD->ToggleHud();
	}
}

void AScreeInPlayerController::ExitScreen()
{
	if (ControlledCharactersStack.IsEmpty())
	{
		return;
	}

	bIsTransitioning = true;

	// disable current character movement and store last rotation
	AScreeInCharacter* CurrentCharacter = GetPawn<AScreeInCharacter>();
	CurrentCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	CurrentCharacter->StoreLastRotation(GetControlRotation());
	CurrentCharacter->ToggleVisibleInGame(false);

	// change possession to the previous character
	AScreeInCharacter* ScreeInCharacter = ControlledCharactersStack.Pop();
	AScreen* ExitScreen = ScreenStack.Pop();
	
	Possess(ScreeInCharacter);

	// disable current character movement for now
	GetPawn<ACharacter>()->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetPawn<ACharacter>()->GetCharacterMovement()->DisableMovement();
	SetIgnoreLookInput(true);
	SetControlRotation(ScreeInCharacter->LastRotation);
	
	SetViewTarget(ExitScreen);
	if (ATelevision* Television = Cast<ATelevision>(ExitScreen))
	{
		// do a camera blend out
		SetViewTargetWithBlend(Television->SeatViewTarget, Television->ScreenEnterDelay, VTBlend_EaseOut, 2.f);
		
		// delay the unlock of entering screen
		GetWorldTimerManager().SetTimer(
			ExitScreenTimerHandle,
			[this, Television]()
			{
				Television->OnFinishSeatViewTransition(this);
				Television->TogglePostProcessVolume(false);
				bIsTransitioning = false;
			},
			Television->ScreenEnterDelay,
			false);

		OnExitToSeatView.Broadcast();
	}
	else
	{
		// do a camera blend out
		SetViewTargetWithBlend(ScreeInCharacter, ExitScreen->ScreenEnterDelay, VTBlend_EaseOut, 2.f);

		// delay the unlock of entering screen
		GetWorldTimerManager().SetTimer(
			ExitScreenTimerHandle,
			[this, ScreeInCharacter]()
			{
				// ExitScreen->TogglePostProcessVolume(false);
				bIsTransitioning = false;
				ScreeInCharacter->ToggleVisibleInGame(true);
				SetIgnoreLookInput(false);

				AScreeInHUD* HUD = GetHUD<AScreeInHUD>();
				if (HUD)
				{
					HUD->ToggleHud();
				}
			},
			ExitScreen->ScreenEnterDelay,
			false);
	}

	AScreeInHUD* HUD = GetHUD<AScreeInHUD>();
	if (HUD)
	{
		HUD->ToggleHud();
	}
}

void AScreeInPlayerController::EnterSeatView(ATelevision* Television)
{
	if (bIsTransitioning) return;
	
	bIsTransitioning = true;
	AScreeInCharacter* ScreeInCharacter = GetPawn<AScreeInCharacter>();
	ScreeInCharacter->CurrentTelevision = Television;
	ScreeInCharacter->CurrentRemoteControl = Television->RemoteControl;
	ScreeInCharacter->ToggleVisibleInGame(false);
	SetIgnoreLookInput(true);

	SetViewTargetWithBlend(Television->SeatViewTarget, Television->SeatViewTransitionDelay, VTBlend_EaseOut, 2.f);
	OnEnterSeatView.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(
		EnterSeatTimerHandle,
		[this, Television]()
		{
			Television->OnFinishSeatViewTransition(this);
			bIsTransitioning = false;
		},
		Television->SeatViewTransitionDelay,
		false);

	AScreeInHUD* HUD = GetHUD<AScreeInHUD>();
	if (HUD)
	{
		HUD->ToggleHud();
	}
}

void AScreeInPlayerController::ExitSeat()
{
	AScreeInCharacter* ScreeInCharacter = GetPawn<AScreeInCharacter>();
	if (!bIsTransitioning && ScreeInCharacter && ScreeInCharacter->CurrentTelevision)
	{
		bIsTransitioning = true;
		SetViewTargetWithBlend(ScreeInCharacter, ScreeInCharacter->CurrentTelevision->SeatViewTransitionDelay, VTBlend_EaseIn, 2.f);
		GetWorld()->GetTimerManager().SetTimer(
			ExitSeatTimerHandle,
			[this, ScreeInCharacter]()
			{
				ScreeInCharacter->ToggleVisibleInGame(true);
				SetIgnoreLookInput(false);
				bIsTransitioning = false;

				AScreeInHUD* HUD = GetHUD<AScreeInHUD>();
				if (HUD)
				{
					HUD->ToggleHud();
				}
			},
			ScreeInCharacter->CurrentTelevision->SeatViewTransitionDelay,
			false);
		
		ScreeInCharacter->CurrentTelevision = nullptr;
		ScreeInCharacter->CurrentRemoteControl = nullptr;
	}
}

void AScreeInPlayerController::AddItemToInventory(UPickupItemData* ItemData)
{
	Inventory.Add(ItemData);
	if (InventoryIndex < 0)
	{
		InventoryIndex = 0;
	}
	
	OnInventoryAddItem.Broadcast(ItemData);
}

void AScreeInPlayerController::RemoveItemFromInventory(UPickupItemData* ItemData)
{
	Inventory.Remove(ItemData);
	if (InventoryIndex >= Inventory.Num())
	{
		InventoryIndex = Inventory.Num() - 1;
	}
	OnInventoryRemoveItem.Broadcast(ItemData);
}

void AScreeInPlayerController::ChangeInventoryIndex(int32 Delta)
{
	if (Inventory.Num() == 0)
	{
		return;
	}

	if (InventoryIndex + Delta < Inventory.Num() && InventoryIndex + Delta >= 0)
	{
		InventoryIndex += Delta;
		OnInventoryIndexChange.Broadcast(InventoryIndex);
	}
}

UPickupItemData* AScreeInPlayerController::GetSelectedInventoryItem()
{
	if (Inventory.IsValidIndex(InventoryIndex))
	{
		return Inventory[InventoryIndex];
	}

	return nullptr;
}

bool AScreeInPlayerController::ItemInInventory(UPickupItemData* ItemData)
{
	return Inventory.Contains(ItemData);
}

void AScreeInPlayerController::StartTransitionToParkingLot()
{
	ClientSetCameraFade(
		true,
		FColor::White,
		FVector2D(-1.0f, 1.0f),
		1.0f,
		false,
		true);

	GetWorld()->GetTimerManager().SetTimer(
		CameraFadeTimerHandle,
		[this]()
		{
			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			LatentInfo.ExecutionFunction = "OnParkingLotLoaded";
			LatentInfo.Linkage = 0;
			LatentInfo.UUID = 0;
			UGameplayStatics::LoadStreamLevel(
				this,
				"ParkinglotMap",
				true,
				false,
				LatentInfo);
		},
		1.0f,
		false);
}

void AScreeInPlayerController::StartTransitionToSecondMainRoom()
{
	ClientSetCameraFade(
		true,
		FColor::White,
		FVector2D(-1.0f, 1.0f),
		1.0f,
		false,
		true);
	
	GetWorld()->GetTimerManager().SetTimer(
		CameraFadeTimerHandle,
		[this]()
		{
			ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(this, "ThirdPersonMap");
			LevelStreaming->SetShouldBeVisible(true);
			
			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			LatentInfo.ExecutionFunction = "OnParkingLotUnloaded";
			LatentInfo.Linkage = 0;
			LatentInfo.UUID = 0;
			UGameplayStatics::UnloadStreamLevel(this, FName("ParkinglotMap"), LatentInfo, false);
		},
		1.0f,
		false);
}

void AScreeInPlayerController::OnParkingLotLoaded()
{
	ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(this, "ThirdPersonMap");
	LevelStreaming->SetShouldBeVisible(false);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, APlayerStart::StaticClass(), FName("ParkingLot"), FoundActors);
	if (FoundActors.Num() > 0)
	{
		AActor* PlayerStart = FoundActors[0];
		GetCharacter()->TeleportTo(PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
	}

	ClientSetCameraFade(
		true,
		FColor::White,
		FVector2D(-1.0f, 0.0f),
		1.0f,
		false,
		false);
}

void AScreeInPlayerController::OnParkingLotUnloaded()
{
	GetCharacter()->TeleportTo(SecondMainRoomPlayerStart->GetActorLocation(), SecondMainRoomPlayerStart->GetActorRotation());
	OnTransitionToMainRoom.Broadcast();
	ClientSetCameraFade(
		true,
		FColor::White,
		FVector2D(-1.0f, 0.0f),
		1.0f,
		false,
		false);
}
