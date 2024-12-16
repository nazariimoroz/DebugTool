// TTVideoGenerator by Nazarii Moroz

#include "Slate/DT_LoggerTabSlate.h"

#include "DebugTool/DT_Logger.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Views/SListView.h"
#include "Styling/CoreStyle.h"
#include "Engine/Font.h"

struct SDT_LoggerTabSlate_LogInfo
{
    FString Log;
    FString NLAfterText;
    TOptional<bool> NLSetted;
    int32 NLIter;
};

void SDT_LoggerTabSlate::Construct(const FArguments& InArgs)
{
    MonoFont = LoadObject<UFont>(nullptr, TEXT("/DebugTool/UI/Fonts/F_Mono.F_Mono"));
    if (!MonoFont)
    {
        DT_ERROR_NO_LOGGER("Cant get MonoFont");
        return;
    }

    ChildSlot
    [
        SNew(SBorder)
        .Padding(FMargin(10))
        [
            SNew(SVerticalBox)

            // TOP BAR
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox)

                // Left Refresh Button
                + SHorizontalBox::Slot().AutoWidth().Padding(5)
                [
                    SNew(SButton)
                    .ButtonStyle(&FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"))
                    .ContentPadding(FMargin(10, 5))
                    .OnClicked(this, &SDT_LoggerTabSlate::OnRefreshClicked)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString("Refresh"))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
                    ]
                ]

                // B1 - White button (no text), toggle enabled/disabled
                + SHorizontalBox::Slot().AutoWidth().Padding(5)
                [
                    MakeToggleButton(
                        FLinearColor(1,1,1,1),   // Enabled color (lighter white)
                        FLinearColor(0.3f,0.3f,0.3f,1), // Disabled color (darker gray)
                        TAttribute<bool>::Create([this]() { return bB1Enabled; }),
                        FOnClicked::CreateSP(this, &SDT_LoggerTabSlate::OnB1Clicked)
                    )
                ]

                // B2 - Yellow button (no text)
                + SHorizontalBox::Slot().AutoWidth().Padding(5)
                [
                    MakeToggleButton(
                        FLinearColor(1,1,0,1),     // Enabled yellow
                        FLinearColor(0.3f,0.3f,0,1), // Disabled darker yellow
                        TAttribute<bool>::Create([this]() { return bB2Enabled; }),
                        FOnClicked::CreateSP(this, &SDT_LoggerTabSlate::OnB2Clicked)
                    )
                ]

                // B3 - Red button (no text)
                + SHorizontalBox::Slot().AutoWidth().Padding(5)
                [
                    MakeToggleButton(
                        FLinearColor(1,0,0,1),     // Enabled red
                        FLinearColor(0.3f,0,0,1), // Disabled darker red
                        TAttribute<bool>::Create([this]() { return bB3Enabled; }),
                        FOnClicked::CreateSP(this, &SDT_LoggerTabSlate::OnB3Clicked)
                    )
                ]

                // Font label and red input field
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .Padding(15, 0, 0, 0)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("Font:"))
                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                ]

                + SHorizontalBox::Slot().AutoWidth().Padding(5)
                [
                    SNew(SEditableTextBox)
                    .Text(FText::FromString("10"))
                    .MinDesiredWidth(20)
                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                ]

                + SHorizontalBox::Slot().AutoWidth().Padding(5)
                [
                    SNew(SEditableTextBox)
                    .HintText(FText::FromString("Filename"))
                    .MinDesiredWidth(100)
                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                ]

                // Right Refresh Button
                + SHorizontalBox::Slot().AutoWidth().Padding(5)
                [
                    SNew(SButton)
                    .ButtonStyle(&FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"))
                    .ContentPadding(FMargin(10, 5))
                    .OnClicked(this, &SDT_LoggerTabSlate::OnClearClicked)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString("Clear"))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
                    ]
                ]
            ]

            // SPACER
            + SVerticalBox::Slot().AutoHeight().Padding(5)
            [
                SNew(SSeparator)
            ]

            // MAIN LIST AREA
            + SVerticalBox::Slot().FillHeight(1.0f).Padding(5)
            [
                SNew(SScrollBox)
                + SScrollBox::Slot()
                [
                    SNew(SVerticalBox)
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    [
                        GenerateLoggerListWidget()
                    ]
                ]
            ]
        ]
    ];
}

TSharedRef<SWidget> SDT_LoggerTabSlate::MakeToggleButton(
    TAttribute<FLinearColor> EnabledColor,
    TAttribute<FLinearColor> DisabledColor,
    TAttribute<bool> State,
    FOnClicked OnClicked)
{
    auto GetButtonColor = [EnabledColor, DisabledColor, State]() {
        return State.Get() ? EnabledColor.Get() : DisabledColor.Get();
    };

    return SNew(SButton)
        .ButtonColorAndOpacity_Lambda(GetButtonColor)
        .ContentPadding(FMargin(8))
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        .OnClicked(OnClicked)
        [
            SNew(STextBlock)
            .Text(FText::GetEmpty())
        ];
}

FReply SDT_LoggerTabSlate::OnB1Clicked()
{
    bB1Enabled = !bB1Enabled;
    return FReply::Handled();
}

FReply SDT_LoggerTabSlate::OnB2Clicked()
{
    bB2Enabled = !bB2Enabled;
    return FReply::Handled();
}

FReply SDT_LoggerTabSlate::OnB3Clicked()
{
    bB3Enabled = !bB3Enabled;
    return FReply::Handled();
}

FReply SDT_LoggerTabSlate::OnRefreshClicked()
{
    LoggerListBox->ClearChildren();
    GenerateLoggerListWidget();
    return FReply::Handled();
}

FReply SDT_LoggerTabSlate::OnClearClicked()
{
    LoggerListBox->ClearChildren();
    return FReply::Handled();
}

TSharedRef<SWidget> SDT_LoggerTabSlate::MakeBlueSquareButton(const FString& ButtonLabel)
{
    return SNew(SButton)
        .ButtonColorAndOpacity(FLinearColor::Blue)
        .ContentPadding(FMargin(8))
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        //.OnClicked(this, &SDT_LoggerTabSlate::OnBlueButtonClicked, ButtonLabel)
        [
            SNew(STextBlock)
            .Text(FText::FromString(ButtonLabel))
            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
            .ColorAndOpacity(FLinearColor::White)
        ];
}


TSharedRef<SWidget> SDT_LoggerTabSlate::GenerateLoggerListWidget()
{
    LoggerListBox = LoggerListBox.Get() ? LoggerListBox : SNew(SVerticalBox);

    if (const auto Logger = UDT_Logger::Get())
    {
        const auto Items = Logger->GetLastLogsInGame();
        for (const auto& Item : Items)
        {
            TSharedPtr<SDT_LoggerTabSlate_LogInfo> LogInfo{ new SDT_LoggerTabSlate_LogInfo{} };
            LogInfo->Log = Item.GetFullText();

            int32 NLIter = 0;
            if (LogInfo->Log.FindChar('\n', NLIter))
            {
                LogInfo->NLSetted = false;
                LogInfo->NLIter = NLIter;
                LogInfo->NLAfterText = LogInfo->Log.Mid(NLIter + 1);
                LogInfo->Log = LogInfo->Log.Left(NLIter);
                LogInfo->Log += " ...";
            }

            const auto ColorLambda = [this, Item]() {
                switch (Item.GetLogVerbosity())
                {
                    case ELogVerbosity::Log : return FLinearColor(0.f,0.f,0.f,1.f);
                    case ELogVerbosity::Warning: return FLinearColor(0.3f,0.3f,0.f,1.f);
                    case ELogVerbosity::Error: return FLinearColor(0.3f,0.f,0.f,1.f);
                    default: return FLinearColor(0.f,0.f,0.f,0.f);
                }
            };

            LoggerListBox->AddSlot()
            .AutoHeight()
            .Padding(2)
            [
                SNew(SButton)
                .ButtonColorAndOpacity_Lambda(ColorLambda)
                .ContentPadding(FMargin(3))
                .OnClicked_Lambda([LogInfo] {
                    if (!LogInfo->NLSetted.IsSet())
                        return FReply::Handled();

                    *LogInfo->NLSetted = !*LogInfo->NLSetted;

                    if (*LogInfo->NLSetted)
                    {
                        LogInfo->Log.RemoveFromEnd(" ...");
                        LogInfo->Log += '\n';
                        LogInfo->Log += LogInfo->NLAfterText;
                        return FReply::Handled();
                    }

                    LogInfo->Log = LogInfo->Log.Left(LogInfo->NLIter);
                    LogInfo->Log += " ...";
                    return FReply::Handled();
                })
                [
                    SNew(STextBlock)
                    .Text_Lambda([LogInfo]{ return FText::FromString(LogInfo->Log); })
                    .Font(FSlateFontInfo(Cast<UObject>(MonoFont), 10))
                ]
            ];
        }
    }

    return LoggerListBox.ToSharedRef();
}
