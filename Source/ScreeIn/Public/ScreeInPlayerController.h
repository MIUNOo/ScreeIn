// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Screen.h"
#include "GameFramework/PlayerController.h"
#include "ScreeInPlayerController.generated.h"

class UInputAction;
class ATelevision;
class UPickupItemData;
class AScreen;
class AScreeInCharacter;
class APlayerStart;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryAddItem, UPickupItemData*, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryRemoveItem, UPickupItemData*, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryIndexChange, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterSeatView);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitToSeatView);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTransitionToMainRoom);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnterScreen, int32, Channel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartEnterScreen);

/**
 * 
 */
UCLASS()
class SCREEIN_API AScreeInPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* PauseAction;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideMainMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void TogglePauseMenu();

private:
	TObjectPtr<UUserWidget> MainMenuWidget;
	TObjectPtr<UUserWidget> PauseMenuWidget;
	bool bIsPauseMenuVisible = false;
	bool bGameStarted = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void EnterScreen(AScreeInCharacter* NewCharacter, AScreen* Screen);

	void EnterScreen(AScreen* Screen);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ExitScreen();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void AddItemToInventory(UPickupItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void RemoveItemFromInventory(UPickupItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ChangeInventoryIndex(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	UPickupItemData* GetSelectedInventoryItem();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool ItemInInventory(UPickupItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartTransitionToParkingLot();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartTransitionToSecondMainRoom();

	UFUNCTION()
	void OnParkingLotLoaded();

	UFUNCTION()
	void OnParkingLotUnloaded();

public:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Gameplay")
	FOnInventoryAddItem OnInventoryAddItem;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Gameplay")
	FOnInventoryRemoveItem OnInventoryRemoveItem;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Gameplay")
	FOnInventoryIndexChange OnInventoryIndexChange;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Gameplay")
	FOnEnterSeatView OnEnterSeatView;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Gameplay")
	FOnExitToSeatView OnExitToSeatView;
	
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Gameplay")
	FOnTransitionToMainRoom OnTransitionToMainRoom;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Gameplay")
	FOnEnterScreen OnEnterScreen;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Gameplay")
	FOnStartEnterScreen OnStartEnterScreen;

	void EnterTelevision(ATelevision* Television);
	void EnterSeatView(ATelevision* Television);
	void ExitSeat();

	TArray<TObjectPtr<AScreeInCharacter>> ControlledCharactersStack;
	TArray<TObjectPtr<AScreen>> ScreenStack;
	bool bCanEnterScreen = true;

	TArray<TObjectPtr<UPickupItemData>> Inventory;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	FTimerHandle EnterScreenTimerHandle;
	FTimerHandle ExitScreenTimerHandle;
	FTimerHandle EnterSeatTimerHandle;
	FTimerHandle ExitSeatTimerHandle;
	FTimerHandle CameraFadeTimerHandle;
	bool bIsTransitioning;
	int32 InventoryIndex;
	TObjectPtr<APlayerStart> SecondMainRoomPlayerStart;
};
