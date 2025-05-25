// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScreeInCharacter.h"

#include "AkComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ExitTrigger.h"
#include "InputActionValue.h"
#include "ScreeInPlayerController.h"
#include "Television.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionInterface.h"
#include "RemoteControl.h"
#include "ScreeInHUD.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AScreeInCharacter

AScreeInCharacter::AScreeInCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;

	bUseControllerRotationRoll = false;

	bUseControllerRotationYaw = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetAutoActivate(false);

	// Create a head camera
	HeadCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HeadCamera"));
	HeadCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
	HeadCamera->SetupAttachment(GetMesh(), FName("Head"));
	HeadCamera->SetAutoActivate(true);

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(HeadCamera);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	AkAudioComponent = CreateDefaultSubobject<UAkComponent>(TEXT("AkAudioComponent"));
	AkAudioComponent->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AScreeInCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AScreeInCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AScreeInCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AScreeInCharacter::Look);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AScreeInCharacter::Interact);
		EnhancedInputComponent->BindAction(ExitAction, ETriggerEvent::Triggered, this, &AScreeInCharacter::ExitSeat);
		EnhancedInputComponent->BindAction(ExitAction, ETriggerEvent::Completed, this, &AScreeInCharacter::ExitScreen);
		EnhancedInputComponent->BindAction(ChannelUpAction, ETriggerEvent::Triggered, this, &AScreeInCharacter::ChannelUp);
		EnhancedInputComponent->BindAction(ChannelDownAction, ETriggerEvent::Triggered, this, &AScreeInCharacter::ChannelDown);
		EnhancedInputComponent->BindAction(MoveHoveredChannelAction, ETriggerEvent::Triggered, this, &AScreeInCharacter::MoveHoveredChannel);
		EnhancedInputComponent->BindAction(SelectChannelAction, ETriggerEvent::Triggered, this, &AScreeInCharacter::SelectChannel);
		EnhancedInputComponent->BindAction(InputNumberAction, ETriggerEvent::Triggered, this, &AScreeInCharacter::InputNumber);
		EnhancedInputComponent->BindAction(TurnTelevisionPowerAction, ETriggerEvent::Triggered, this, &AScreeInCharacter::TurnTelevisionPower);
		EnhancedInputComponent->BindAction(SelectInventoryItemAction, ETriggerEvent::Triggered, this, &AScreeInCharacter::SelectInventoryItem);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

FRotator AScreeInCharacter::GetHeadRotation() const
{
	return HeadCamera->GetComponentRotation();
}

void AScreeInCharacter::StoreLastRotation(const FRotator& Rotation)
{
	LastRotation = Rotation;
}

void AScreeInCharacter::ToggleVisibleInGame(bool bVisible)
{
	GetMesh()->SetVisibility(bVisible);
	GetCapsuleComponent()->SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = bVisible ? 1.0f : 0.0f;
	GetCharacterMovement()->SetMovementMode(bVisible ? MOVE_Walking : MOVE_None);
}

void AScreeInCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AScreeInCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AScreeInCharacter::BeginPlay()
{
	Super::BeginPlay();

	LastRotation = GetHeadRotation();
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC || PC->GetPawn() != this)
	{
		GetMesh()->SetVisibility(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetSimulatePhysics(false);
	}
}

void AScreeInCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetCharacterMovement()->IsFalling() && !bIsPlayingFallingSound && StartFallingHeight - GetActorLocation().Z > FallingSoundThreshold)
	{
		AkAudioComponent->PostAkEvent(FallingSound);
		bIsPlayingFallingSound = true;
	}
}

void AScreeInCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	AkAudioComponent->Stop();
	bIsPlayingFallingSound = false;
	if (GetCharacterMovement()->IsFalling())
	{
		StartFallingHeight = GetActorLocation().Z;
	} else if (PrevMovementMode == MOVE_Falling && GetCharacterMovement()->IsMovingOnGround())
	{
		if (StartFallingHeight - GetActorLocation().Z > LandingSoundThreshold)
		{
			AkAudioComponent->PostAkEvent(LandingSound);
		}
	}
}

void AScreeInCharacter::InteractTrace(float Range)
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	//Get the start and end location of the trace
	FVector TraceStartLocation;
	FVector TraceEndLocation;

	TraceStartLocation = CameraManager->GetCameraLocation();
	TraceEndLocation = TraceStartLocation + (Range * CameraManager->GetActorForwardVector());

	FCollisionQueryParams CollisionParams;
	FHitResult Hit;

	//Ignore the player
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStartLocation, TraceEndLocation, ECollisionChannel::ECC_Visibility, CollisionParams);

	AActor* LookAtActor = nullptr;
	FString LookAtMessage = "";

	if (bHit)
	{

		//IInteractionInterface* InteractionObject = Cast<IInteractionInterface>(Hit.GetActor());
		if (Hit.GetActor()->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_LookAt(Hit.GetActor(), this, GetController<AScreeInPlayerController>(), Hit.GetComponent(), LookAtActor, LookAtComponent, LookAtMessage);
			AScreeInHUD* HUD = Cast<AScreeInHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

			if (HUD)
			{
				if (LookAtComponent)
				{
					HUD->ChangeCrosshairAlpha(1.0f);
				}
				HUD->DisplayMessage(LookAtMessage);
			}

		}
		else
		{
			LookAtComponent = nullptr;
			AScreeInHUD* HUD = Cast<AScreeInHUD>( GetWorld()->GetFirstPlayerController()->GetHUD());
			if (HUD)
			{
				HUD->ChangeCrosshairAlpha(.5f);
				HUD->DisplayMessage("");
			}
			//HUD
		}
	}
	else
	{
		LookAtComponent = nullptr;
		//HUD
		if (AScreeInHUD* HUD = Cast<AScreeInHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			HUD->ChangeCrosshairAlpha(.5f);
			HUD->DisplayMessage("");
		}
	}

}

void AScreeInCharacter::Interact(const FInputActionValue& Value)
{
	// is in the seat view of a television
	if (CurrentTelevision)
	{
		EnterScreen(CurrentTelevision);
		return;
	}
	
	if (LookAtComponent!=nullptr)
	{
		// if interact with a television
		if (ATelevision* Television = Cast<ATelevision>(LookAtComponent->GetOwner()))
		{
			if (AScreeInPlayerController* ScreeInPlayerController = GetController<AScreeInPlayerController>())
			{
				ScreeInPlayerController->EnterSeatView(Television);
			}
		}
		else if (AScreen* Screen = Cast<AScreen>(LookAtComponent->GetOwner()))
		{
			if (AScreeInPlayerController* ScreeInPlayerController = GetController<AScreeInPlayerController>())
			{
				ScreeInPlayerController->EnterScreen(Screen);
			}
		}
		else
		{
			bool bIsInteractable = false;
			IInteractionInterface::Execute_InteractWith(LookAtComponent->GetOwner(), this, LookAtComponent, bIsInteractable);
		}
	}
}

void AScreeInCharacter::EnterScreen(ATelevision* Television)
{
	if (AScreeInPlayerController* ScreeInPlayerController = GetController<AScreeInPlayerController>())
	{
		ScreeInPlayerController->EnterTelevision(Television);
		// Television->TriggerEnterScreen(ScreeInPlayerController);
	}
}

void AScreeInCharacter::ExitSeat(const FInputActionValue& Value)
{
	// if in the seat view of a television
	// if (!bIsTransitioning && CurrentTelevision)
	// {
		if (AScreeInPlayerController* ScreeInPlayerController = GetController<AScreeInPlayerController>())
		{
			ScreeInPlayerController->ExitSeat();
			// CurrentTelevision->bCanEnterScreen = false;
			// // CurrentRemoteControl->ToggleHoveringVisible(false);
			// ScreeInPlayerController->SetViewTargetWithBlend(
			// 	this, CurrentTelevision->SeatViewTransitionDelay, VTBlend_EaseIn, 2.f);
			// bIsTransitioning = true;
			// GetWorld()->GetTimerManager().SetTimer(
			// 	ExitSeatTimerHandle,
			// 	[this, ScreeInPlayerController]()
			// 	{
			// 		GetMesh()->SetVisibility(true);
			// 		ScreeInPlayerController->SetIgnoreLookInput(false);
			// 		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			// 		bIsTransitioning = false;
			// 	},
			// 	CurrentTelevision->SeatViewTransitionDelay,
			// 	false);
			//
			// CurrentTelevision = nullptr;
			// CurrentRemoteControl = nullptr;
			// AScreeInHUD* HUD = Cast<AScreeInHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			// if (HUD)
			// {
			// 	HUD->ToggleHud();
			// }
		}
	// }
}

void AScreeInCharacter::ExitScreen(const FInputActionValue& Value)
{
	if (LookAtComponent)
	{
		if (LookAtComponent->GetOwner()->IsA(AExitTrigger::StaticClass()))
		{
			if (AScreeInPlayerController* ScreeInPlayerController = GetController<AScreeInPlayerController>())
			{
				ScreeInPlayerController->ExitScreen();
			}
		}
	}
}

void AScreeInCharacter::ChannelUp(const FInputActionValue& Value)
{
	// if (AScreeInPlayerController* ScreeInPlayerController = GetController<AScreeInPlayerController>())
	// {
	// 	if (ScreeInPlayerController->ControlledCharactersStack.Num() > 0)
	// 	{
	// 		return;
	// 	}
	// }

	// if (CurrentTelevision)
	// {
	// 	CurrentTelevision->ChannelUp();
	// }
}

void AScreeInCharacter::ChannelDown(const FInputActionValue& Value)
{
	// if (AScreeInPlayerController* ScreeInPlayerController = GetController<AScreeInPlayerController>())
	// {
	// 	if (ScreeInPlayerController->ControlledCharactersStack.Num() > 0)
	// 	{
	// 		return;
	// 	}
	// }
	//
	// if (CurrentTelevision)
	// {
	// 	CurrentTelevision->ChannelDown();
	// }
}

void AScreeInCharacter::MoveHoveredChannel(const FInputActionValue& Value)
{
	if (CurrentRemoteControl)
	{
		FVector2D DirectionVector = Value.Get<FVector2D>();
		int X = FMath::RoundToInt(DirectionVector.X);
		int Y = FMath::RoundToInt(DirectionVector.Y);
		// CurrentRemoteControl->MoveHoveredNumber(X, Y);
	}
}

void AScreeInCharacter::SelectChannel(const FInputActionValue& Value)
{
	if (CurrentRemoteControl)
	{
		// CurrentRemoteControl->SelectHoveredNumber();
	}
}

void AScreeInCharacter::InputNumber(const FInputActionValue& Value)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = nullptr;
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	}
	
	if (CurrentRemoteControl && Controller != nullptr && Subsystem)
	{
		TArray<FKey> Keys = Subsystem->QueryKeysMappedToAction(InputNumberAction);
		for (const FKey& Key : Keys)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			if (PlayerController->IsInputKeyDown(Key)) // Check which key is actively pressed
			{
				int NumberPressed = -1;
				if (Key == EKeys::Zero || Key == EKeys::NumPadZero) NumberPressed = 0;
				else if (Key == EKeys::One || Key == EKeys::NumPadOne) NumberPressed = 1;
				else if (Key == EKeys::Two || Key == EKeys::NumPadTwo) NumberPressed = 2;
				else if (Key == EKeys::Three || Key == EKeys::NumPadThree) NumberPressed = 3;
				else if (Key == EKeys::Four || Key == EKeys::NumPadFour) NumberPressed = 4;
				else if (Key == EKeys::Five || Key == EKeys::NumPadFive) NumberPressed = 5;
				else if (Key == EKeys::Six || Key == EKeys::NumPadSix) NumberPressed = 6;
				else if (Key == EKeys::Seven || Key == EKeys::NumPadSeven) NumberPressed = 7;
				else if (Key == EKeys::Eight || Key == EKeys::NumPadEight) NumberPressed = 8;
				else if (Key == EKeys::Nine || Key == EKeys::NumPadNine) NumberPressed = 9;
				
				// int32 Number = KeyToNumber(Key); // Convert FKey to 0-9
				// Execute custom logic with the Number
				CurrentRemoteControl->InputNumberKey(NumberPressed);
			}
		}
	}
}

void AScreeInCharacter::TurnTelevisionPower(const FInputActionValue& Value)
{
	if (CurrentRemoteControl)
	{
		CurrentRemoteControl->TurnTelevisionPower();
	}
}

void AScreeInCharacter::SelectInventoryItem(const FInputActionValue& Value)
{
	float ScrollValue = Value.Get<float>();

	AScreeInPlayerController* ScreeInPlayerController = GetController<AScreeInPlayerController>();
	if (ScreeInPlayerController)
	{
		if (ScrollValue > 0)
		{
			ScreeInPlayerController->ChangeInventoryIndex(1);
		}
		else if (ScrollValue < 0)
		{
			ScreeInPlayerController->ChangeInventoryIndex(-1);
		}
	}
	
}

FRotator AScreeInCharacter::GetViewRotation() const
{
	if (Controller != nullptr)
	{
		return Controller->GetControlRotation();
	}

	return LastRotation;
}
