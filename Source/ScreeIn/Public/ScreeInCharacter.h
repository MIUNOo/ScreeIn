// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ScreeInCharacter.generated.h"

class UAkComponent;
class UAkAudioEvent;
class ARemoteControl;
class ATelevision;
class AScreen;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AScreeInCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCapture;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ExitAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChannelUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChannelDownAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveHoveredChannelAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectChannelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InputNumberAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TurnTelevisionPowerAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectInventoryItemAction;

	/** Head camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* HeadCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPrimitiveComponent* LookAtComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	UAkComponent* AkAudioComponent;

public:
	AScreeInCharacter();

	FRotator LastRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	UAkAudioEvent* FallingSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	UAkAudioEvent* LandingSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float LandingSoundThreshold = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float FallingSoundThreshold = 150.f;

	UFUNCTION()
	void StoreLastRotation(const FRotator& Rotation);

	void ToggleVisibleInGame(bool bVisible);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnRuleViolated();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractTrace(float Range);

	void Interact(const FInputActionValue& Value);
	void EnterScreen(ATelevision* Television);
	void ExitSeat(const FInputActionValue& Value);
	void ExitScreen(const FInputActionValue& Value);
	void ChannelUp(const FInputActionValue& Value);
	void ChannelDown(const FInputActionValue& Value);
	void MoveHoveredChannel(const FInputActionValue& Value);
	void SelectChannel(const FInputActionValue& Value);
	void InputNumber(const FInputActionValue& Value);
	void TurnTelevisionPower(const FInputActionValue& Value);
	void SelectInventoryItem(const FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetViewRotation() const override;
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FRotator GetHeadRotation() const;

	TObjectPtr<ATelevision> CurrentTelevision;
	TObjectPtr<ARemoteControl> CurrentRemoteControl;

private:
	
	float StartFallingHeight;
	bool bIsPlayingFallingSound = false;
	
	bool bIsTransitioning = false;
};
