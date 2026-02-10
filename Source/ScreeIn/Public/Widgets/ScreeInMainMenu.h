#pragma once

#include "CoreMinimal.h"
#include "Television.h"
#include "Blueprint/UserWidget.h"
#include "ScreeInMainMenu.generated.h"

class UPanelWidget;

UCLASS()
class SCREEIN_API UScreeInMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Television")
	TObjectPtr<ATelevision> TargetTelevision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Television")
	FName TelevisionTag = FName("MainMenuScreen");

protected:
	virtual void NativeConstruct() override;

private:
	void InitializeButtons();
	void FindTelevisionByTag();
	void GetAllChildButtons(UPanelWidget* Panel, TArray<UWidget*>& OutWidgets);
};