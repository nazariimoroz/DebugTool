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
                        FLinearColor(1,1,1,1),
                        FLinearColor(0.3f,0.3f,0.3f,1),
                        TAttribute<bool>::Create([this]() { return bB1Enabled; }),
                        FOnClicked::CreateSP(this, &SDT_LoggerTabSlate::OnB1Clicked)
                    )
                ]

                // B2 - Yellow button (no text)
                + SHorizontalBox::Slot().AutoWidth().Padding(5)
                [
                    MakeToggleButton(
                        FLinearColor(1,1,0,1),
                        FLinearColor(0.3f,0.3f,0,1),
                        TAttribute<bool>::Create([this]() { return bB2Enabled; }),
                        FOnClicked::CreateSP(this, &SDT_LoggerTabSlate::OnB2Clicked)
                    )
                ]

                // B3 - Red button (no text)
                + SHorizontalBox::Slot().AutoWidth().Padding(5)
                [
                    MakeToggleButton(
                        FLinearColor(1,0,0,1),
                        FLinearColor(0.3f,0,0,1),
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
            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            .Padding(5)
            [
                SAssignNew(ListScrollBox, SScrollBox)

                + SScrollBox::Slot()
                [
                    SAssignNew(LoggerListBox, SVerticalBox)
                ]
            ]
        ]
    ];

    GenerateLoggerListWidget();
    ListScrollBox->ScrollToEnd();
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

struct SDT_LoggerTabSlate_LogInfo
{
    FString Log;
    ELogVerbosity::Type Verbosity;

    FString NLAfterText;
    TOptional<bool> NLSetted;
    int32 NLIter;
};

void SDT_LoggerTabSlate::GenerateLoggerListWidget()
{
    const auto Logger = UDT_Logger::Get();

    DT_RETURN_NO_LOGGER(LoggerListBox);
    DT_RETURN_NO_LOGGER(Logger);

    Logger->OnAddLogDelegate.AddSPLambda(this, [this](FDT_LogElement LogInfo) {
        float CurrentOffset = ListScrollBox->GetScrollOffset();
        float EndOffset     = ListScrollBox->GetScrollOffsetOfEnd();

        bool ShouldScroll = FMath::IsNearlyEqual(CurrentOffset, EndOffset, KINDA_SMALL_NUMBER);

        AddItemToLoggerListWidget(LogInfo);

        if (ShouldScroll)
        {
            ListScrollBox->ScrollToEnd();
        }
    });

    const auto Items = Logger->GetLastLogs();
    for (const auto& Item : Items)
    {
        AddItemToLoggerListWidget(Item);
    }
}

void SDT_LoggerTabSlate::AddItemToLoggerListWidget(const FDT_LogElement& LogElement)
{
    if (!LoggerListBox) return;

    LoggerListBox->AddSlot()
    .AutoHeight()
    .Padding(2)
    [
        GenerateLogItemWidget(LogElement)
    ];
}

TSharedRef<SWidget> SDT_LoggerTabSlate::GenerateLogItemWidget(const FDT_LogElement& LogElement)
{
    TSharedPtr<SDT_LoggerTabSlate_LogInfo> LogInfo{new SDT_LoggerTabSlate_LogInfo{}};
    LogInfo->Log = LogElement.GetFullText();
    LogInfo->Verbosity = LogElement.LogVerbosity;

    int32 NLIter = 0;
    if (LogInfo->Log.FindChar('\n', NLIter))
    {
        LogInfo->NLSetted    = false;
        LogInfo->NLIter      = NLIter;
        LogInfo->NLAfterText = LogInfo->Log.Mid(NLIter + 1);
        LogInfo->Log         = LogInfo->Log.Left(NLIter);
        LogInfo->Log         += " ...";
    }

    const auto Color = ([&LogElement]() {
        switch (LogElement.LogVerbosity)
        {
            case ELogVerbosity::Log:
                return FLinearColor(0.f, 0.f, 0.f, 1.f);
            case ELogVerbosity::Warning:
                return FLinearColor(0.3f, 0.3f, 0.f, 1.f);
            case ELogVerbosity::Error:
                return FLinearColor(0.3f, 0.f, 0.f, 1.f);
            default:
                return FLinearColor(0.f, 0.f, 0.f, 0.f);
        }
    })();

    const auto OnClick = [LogInfo]
    {
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
    };

    const auto GetText = [LogInfo]
    {
        return FText::FromString(LogInfo->Log);
    };

    return SNew(SButton)
           .ButtonColorAndOpacity(Color)
           .ContentPadding(FMargin(3))
           .OnClicked_Lambda(OnClick)
            [
               SNew(STextBlock)
               .Text_Lambda(GetText)
               .Font(FSlateFontInfo(Cast<UObject>(MonoFont), 10))
           ];
}
