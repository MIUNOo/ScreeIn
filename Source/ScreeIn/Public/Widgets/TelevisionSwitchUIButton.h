#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "TelevisionSwitchUIButton.generated.h"

class ATelevision;

UCLASS()
class SCREEIN_API UTelevisionSwitchUIButton : public UButton
{
	GENERATED_BODY()

public:
	// 按钮关联的频道号
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Television")
	int32 ChannelNumber = -1;

	void Initialize();
	void Cleanup();

	
private:
	UFUNCTION()
	void HandleHovered();

	UFUNCTION()
	void HandleUnhovered();

	// 获取Television引用(通过MainMenu)
	ATelevision* GetTargetTelevision();

	bool bIsInitialized = false;
	int32 PreviousChannel = -1;
};