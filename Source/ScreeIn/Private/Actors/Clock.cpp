// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Clock.h"

#include "AkComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AClock::AClock()
{
	ClockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockMesh"));
	RootComponent = ClockMesh;

	HourHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HourHand"));
	HourHand->SetupAttachment(ClockMesh);

	MinuteHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MinuteHand"));
	MinuteHand->SetupAttachment(ClockMesh);

	SecondHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondHand"));
	SecondHand->SetupAttachment(ClockMesh);

	AkAudioComponent = CreateDefaultSubobject<UAkComponent>(TEXT("AkAudioComponent"));
	AkAudioComponent->SetupAttachment(RootComponent);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AClock::BeginPlay()
{
	Super::BeginPlay();

	FDateTime Now = FDateTime::Now();
	int32 Hour = Now.GetHour();
	int32 Minute = Now.GetMinute();
	int32 Second = Now.GetSecond();

	FQuat SecondDeltaQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(Second * 6));
	FQuat MinuteDeltaQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(Minute * 6 + Second * 0.1));
	FQuat HourDeltaQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(Hour * 30 + Minute * 0.5 + Second * 0.0083333));

	FQuat CurrentQuat = HourHand->GetRelativeRotation().Quaternion();
	HourHand->SetRelativeRotation(HourDeltaQuat * CurrentQuat);

	CurrentQuat = MinuteHand->GetRelativeRotation().Quaternion();
	MinuteHand->SetRelativeRotation(MinuteDeltaQuat * CurrentQuat);

	CurrentQuat = SecondHand->GetRelativeRotation().Quaternion();
	SecondHand->SetRelativeRotation(SecondDeltaQuat * CurrentQuat);

	GetWorld()->GetTimerManager().SetTimer(SecondHandTimer, this, &AClock::TickSecondHand, 1.f, true);

	// if (bPlayTickSound)
	// {
	// 	AkAudioComponent->PostAkEvent(TickSoundAk);
	// }
}

// Called every frame
void AClock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FQuat CurrentQuat = HourHand->GetRelativeRotation().Quaternion();
	FQuat DeltaQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(0.0083333f * DeltaTime));
	HourHand->SetRelativeRotation(DeltaQuat * CurrentQuat);

	CurrentQuat = MinuteHand->GetRelativeRotation().Quaternion();
	DeltaQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(0.1f * DeltaTime));
	MinuteHand->SetRelativeRotation(DeltaQuat * CurrentQuat);
}

void AClock::TickSecondHand()
{
	FQuat CurrentQuat = SecondHand->GetRelativeRotation().Quaternion();
	FQuat DeltaQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(6.f));
	SecondHand->SetRelativeRotation(DeltaQuat * CurrentQuat);

	if (TickSound && bPlayTickSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, TickSound, GetActorLocation());
	}
}

void AClock::TogglePlayTickSound(bool bPlay)
{
	if (bPlay)
	{
		// AkAudioComponent->PostAkEvent(TickSoundAk);
		bPlayTickSound = true;
	}
}
