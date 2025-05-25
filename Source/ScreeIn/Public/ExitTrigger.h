#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "ExitTrigger.generated.h"

UCLASS()
class SCREEIN_API AExitTrigger : public AActor, public IInteractionInterface
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AExitTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ExitBoard;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	virtual void LookAt_Implementation(const UObject* Interactor, const APlayerController* Controller,
		const UPrimitiveComponent* HitComponent, AActor*& LookAtActor, UPrimitiveComponent*& LookAtComponent,
		FString& Message) override;
	
	virtual void InteractWith_Implementation(const UObject* Interactor, const UPrimitiveComponent* HitComponent,
		bool& Interactable) override;
};
