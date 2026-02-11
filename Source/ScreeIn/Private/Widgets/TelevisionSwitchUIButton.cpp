#include "Widgets/TelevisionSwitchUIButton.h"
#include "Television.h"
#include "Widgets/ScreeInMainMenu.h"

void UTelevisionSwitchUIButton::Initialize()
{
	if (bIsInitialized)
	{
		return;
	}

	OnHovered.AddDynamic(this, &UTelevisionSwitchUIButton::HandleHovered);
	OnUnhovered.AddDynamic(this, &UTelevisionSwitchUIButton::HandleUnhovered);

	bIsInitialized = true;
}

void UTelevisionSwitchUIButton::Cleanup()
{
	if (!bIsInitialized)
	{
		return;
	}

	OnHovered.RemoveDynamic(this, &UTelevisionSwitchUIButton::HandleHovered);
	OnUnhovered.RemoveDynamic(this, &UTelevisionSwitchUIButton::HandleUnhovered);

	bIsInitialized = false;
}

ATelevision* UTelevisionSwitchUIButton::GetTargetTelevision()
{
	// 向上查找父控件,直到找到 ScreeInMainMenu
	UScreeInMainMenu* Menu = GetTypedOuter<UScreeInMainMenu>();
	return Menu ? Menu->TargetTelevision : nullptr;
	
}

void UTelevisionSwitchUIButton::HandleHovered()
{
	ATelevision* Television = GetTargetTelevision();
    
	if (!Television || ChannelNumber < 0)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("TelevisionSwitchUIButton hovered"));
	
	// 保存当前频道
	PreviousChannel = Television->CurrentChannel;
    
	// 切换到这个按钮的频道
	Television->SwitchChannel(ChannelNumber);
}

void UTelevisionSwitchUIButton::HandleUnhovered()
{
	ATelevision* Television = GetTargetTelevision();
    
	if (!Television)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("TelevisionSwitchUIButton Unhovered"));

	// TODO: 恢复到花屏
	Television->SwitchChannel(PreviousChannel);
	Television->OnShowStaticNoise.Broadcast();
}