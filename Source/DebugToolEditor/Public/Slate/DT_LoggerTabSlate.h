#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UFont;

class SDT_LoggerTabSlate : public SCompoundWidget
{
  public:
    SLATE_BEGIN_ARGS(SDT_LoggerTabSlate)
    {
    }
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

  private:
    // State booleans for each button
    bool bB1Enabled = true;
    bool bB2Enabled = true;
    bool bB3Enabled = true;
    UFont* MonoFont;

    // Helper to build a toggleable color button
    TSharedRef<SWidget> MakeToggleButton(TAttribute<FLinearColor> EnabledColor, TAttribute<FLinearColor> DisabledColor,
                                         TAttribute<bool> EnabledState, FOnClicked OnClicked);

    // Separate handlers for each button click
    FReply OnB1Clicked();
    FReply OnB2Clicked();
    FReply OnB3Clicked();

    // Existing code...
    FReply OnRefreshClicked();

    // Existing code...
    TSharedRef<SWidget> MakeBlueSquareButton(const FString& ButtonLabel);
    TSharedRef<SWidget> GenerateListWidget();
};

