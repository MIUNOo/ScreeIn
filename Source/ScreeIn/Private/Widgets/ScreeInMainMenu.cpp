#include "Widgets/ScreeInMainMenu.h"
#include "Widgets/TelevisionSwitchUIButton.h"
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"

void UScreeInMainMenu::NativeConstruct()
{
    Super::NativeConstruct();
    FindTelevisionByTag();
    InitializeButtons();
}

void UScreeInMainMenu::FindTelevisionByTag()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("MainMenu: World is null!"));
        return;
    }

    // 使用 GameplayStatics 通过 Tag 查找
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClassWithTag(
        World,
        ATelevision::StaticClass(),
        TelevisionTag,
        FoundActors
    );

    if (FoundActors.Num() > 0)
    {
        TargetTelevision = Cast<ATelevision>(FoundActors[0]);
        UE_LOG(LogTemp, Log, TEXT("MainMenu found Television with tag '%s': %s"), 
            *TelevisionTag.ToString(), 
            *TargetTelevision->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MainMenu: No Television found with tag '%s'!"), 
            *TelevisionTag.ToString());
    }

    /*TargetTelevision->TurnPowerOn();*/
}

void UScreeInMainMenu::InitializeButtons()
{
    // 递归初始化所有TelevisionSwitchUIButton
    TArray<UWidget*> ChildWidgets;
    
    // 从根面板开始递归查找
    if (UPanelWidget* RootPanel = Cast<UPanelWidget>(GetRootWidget()))
    {
        GetAllChildButtons(RootPanel, ChildWidgets);
    }

    for (UWidget* Widget : ChildWidgets)
    {
        if (UTelevisionSwitchUIButton* TVButton = Cast<UTelevisionSwitchUIButton>(Widget))
        {
            TVButton->Initialize();
        }
    }
}

void UScreeInMainMenu::GetAllChildButtons(UPanelWidget* Panel, TArray<UWidget*>& OutWidgets)
{
    if (!Panel)
    {
        return;
    }

    int32 ChildCount = Panel->GetChildrenCount();
    for (int32 i = 0; i < ChildCount; ++i)
    {
        UWidget* Child = Panel->GetChildAt(i);
        if (Child)
        {
            OutWidgets.Add(Child);
            
            // 如果子控件也是面板,递归查找
            if (UPanelWidget* ChildPanel = Cast<UPanelWidget>(Child))
            {
                GetAllChildButtons(ChildPanel, OutWidgets);
            }
        }
    }
}