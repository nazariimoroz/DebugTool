#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FDT_LogElement;
class UFont;

class SDT_LoggerTabSlate : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SDT_LoggerTabSlate)
  {
  }
  SLATE_END_ARGS()

  void Construct(const FArguments& InArgs);

protected:
    TSharedRef<SWidget> MakeToggleButton(TAttribute<FLinearColor> EnabledColor, TAttribute<FLinearColor> DisabledColor,
                                         TAttribute<bool> EnabledState, FOnClicked OnClicked);

    FReply OnB1Clicked();
    FReply OnB2Clicked();
    FReply OnB3Clicked();

    FReply OnRefreshClicked();

    FReply OnClearClicked();

    TSharedRef<SWidget> MakeBlueSquareButton(const FString& ButtonLabel);

    TSharedRef<SWidget> GenerateLoggerListWidget();
    void AddItemToLoggerListWidget(const FDT_LogElement& LogElement);
    TSharedRef<SWidget> GenerateLogItemWidget(const FDT_LogElement& LogElement);

private:
    bool bB1Enabled = true;
    bool bB2Enabled = true;
    bool bB3Enabled = true;
    UFont* MonoFont;

    TSharedPtr<SVerticalBox> LoggerListBox = nullptr;

};

