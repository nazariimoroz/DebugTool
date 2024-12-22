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
#include "Slate/SMultiSelectComboBox.h"

void SDT_LoggerTabSlate::Construct(const FArguments& InArgs)
{
    MonoFont = LoadObject<UFont>(nullptr, TEXT("/DebugTool/UI/Fonts/F_Mono.F_Mono"));
    if (!MonoFont)
    {
        DT_ERROR_NO_LOGGER("Cant get MonoFont");
        return;
    }

    ShownVerbosity.Add(ELogVerbosity::Display, true);
    ShownVerbosity.Add(ELogVerbosity::Warning, true);
    ShownVerbosity.Add(ELogVerbosity::Error, true);

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

                // Clear Button
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
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


                + SHorizontalBox::Slot()
                .FillWidth(1.f)

                + SHorizontalBox::Slot()
                .HAlign(HAlign_Right)
                .Padding(5)
                .AutoWidth()
                [
                    SAssignNew(MenuAnchor, SMenuAnchor)
                    .OnGetMenuContent(this, &SDT_LoggerTabSlate::GenerateMenuContent)
                    .Placement(MenuPlacement_BelowRightAnchor)
                    .Method(EPopupMethod::CreateNewWindow)
                    [
                        SNew(SButton)
                        .ButtonStyle(&FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"))
                        .OnClicked(this, &SDT_LoggerTabSlate::OnOpenMenuClicked)
                        [
                            SNew(STextBlock)
                            .Text(NSLOCTEXT("Logger", "Tab", "≡"))
                            .Font(FSlateFontInfo(Cast<UObject>(MonoFont), 14))
                        ]
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
            [SNew(STextBlock)
                 .Text(FText::FromString(ButtonLabel))
                 .Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
                 .ColorAndOpacity(FLinearColor::White)];
}

FLinearColor SDT_LoggerTabSlate::GetColorForVerbosity(ELogVerbosity::Type Verbosity)
{
    switch (Verbosity)
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
}

struct SDT_LoggerTabSlate_LogInfo
{
    SDT_LoggerTabSlate_LogInfo(const FDT_LogElement* InLogElement)
        : LogElement(InLogElement)
    {
        switch (LogElement->NetMode)
        {
            case NM_Client:
            {
                NetStatusMessage = FText::FromString(FString::Printf(TEXT("Client %i"), LogElement->NetId));
                break;
            }
            case NM_ListenServer:
            case NM_DedicatedServer:
            {
                NetStatusMessage = FText::FromString(TEXT("Server"));
                break;
            }

            case NM_Standalone:
            {
                NetStatusMessage = FText::FromString(TEXT("Standalone"));
                break;
            }

            case NM_MAX:
            default:
            {
                NetStatusMessage = FText::FromString(TEXT("Unknown"));
                break;
            }
        }

        int32 Index = 0;
        if (LogElement->Message.FindChar('\n', Index))
        {
            bHaveNL = true;
            PrimaryLineMessage = FText::FromString(LogElement->Message.Left(Index));
            NewLineMessage = FText::FromString(LogElement->Message.Mid(Index + 1));
        }
        else
        {
            PrimaryLineMessage = FText::FromString(LogElement->Message);
        }

        if (LogElement->StackTrace)
        {
            bHaveNL = true;

            auto Result = FString::Printf(TEXT("\n\n%s"), **LogElement->StackTrace);
            Result.RemoveAt(Result.Len() - 2, 2); // TODO: windows only, make crossplatform
            NewLineStackTrace = FText::FromString(MoveTempIfPossible(Result));
        }

        FileNameWithLine = FText::FromString(FString::Printf(TEXT("%s(%llu)"), *LogElement->File, LogElement->Line));

		Tag = FText::FromString(LogElement->Tag);
    }

    ELogVerbosity::Type GetLogVerbosity() const
    {
        return LogElement->LogVerbosity;
    }

    bool HaveNL() const
    {
        return bHaveNL;
    }

    bool OpenedNL() const
    {
        return bOpenedNL;
    }
    void SwitchNL()
    {
        if (HaveNL())
        {
            bOpenedNL = !bOpenedNL;
        }
    }

    const FText& GetPrimaryLineMessage() const
    {
        return PrimaryLineMessage;
    }

    const TOptional<FText>& GetNewLineMessage() const
    {
        return NewLineMessage;
    }

    const TOptional<FText>& GetNewLineStackTrace() const
    {
        return NewLineStackTrace;
    }

    const FText& GetFileNameWithLine() const
    {
        return FileNameWithLine;
    }

    const FText& GetNetStatusMessage() const
    {
        return NetStatusMessage;
    }

	const FText& GetTag() const
    {
    	return Tag;
    }

protected:
    const FDT_LogElement* LogElement;
    FText PrimaryLineMessage;

    bool bHaveNL = false;
    bool bOpenedNL = false;
    TOptional<FText> NewLineMessage;
    TOptional<FText> NewLineStackTrace;

    FText FileNameWithLine;
    FText NetStatusMessage;
	FText Tag;

};

void SDT_LoggerTabSlate::GenerateLoggerListWidget()
{
    const auto Logger = UDT_Logger::Get();

    DT_RETURN_NO_LOGGER(LoggerListBox);
    DT_RETURN_NO_LOGGER(Logger);

    Logger->OnAddLogDelegate.AddSPLambda(this, [this](const FDT_LogElement* LogElement) {
        float CurrentOffset = ListScrollBox->GetScrollOffset();
        float EndOffset     = ListScrollBox->GetScrollOffsetOfEnd();

        bool ShouldScroll = FMath::IsNearlyEqual(CurrentOffset, EndOffset, KINDA_SMALL_NUMBER);

        AddItemToLoggerListWidget(*LogElement);

        if (ShouldScroll)
        {
            ListScrollBox->ScrollToEnd();
        }
    });

    for (const auto& Item : *Logger)
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
    TSharedPtr<SDT_LoggerTabSlate_LogInfo> LogInfo{new SDT_LoggerTabSlate_LogInfo{&LogElement}};

    const auto Color = GetColorForVerbosity(LogElement.LogVerbosity);

    const auto OnClick = [LogInfo] {
        LogInfo->SwitchNL();

        return FReply::Handled();
    };

    return SNew(SButton)
        .ButtonColorAndOpacity(Color)
        .ContentPadding(FMargin(3))
        .OnClicked_Lambda(OnClick)
        .Visibility_Lambda([this, LogInfo] {
            if ( ShownVerbosity[LogInfo->GetLogVerbosity()] )
                return EVisibility::Visible;
            return EVisibility::Collapsed;
        })
        [
            SNew(SVerticalBox)

            // Primary line
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox)

                // NetStatus
                + SHorizontalBox::Slot().AutoWidth()
                [
                    SNew(SBox)
                    .WidthOverride(80.f)
                    [
                        SNew(STextBlock)
                        .Visibility_Lambda([LogInfo, this]{
                            if (!bShowNetStatus)
                                return EVisibility::Collapsed;

                            return EVisibility::Visible;
                        })
                        .Text_Lambda([LogInfo] { return LogInfo->GetNetStatusMessage(); })
                        .Font(FSlateFontInfo(Cast<UObject>(MonoFont), 10))
                    ]
                ]

            	// Tag
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(SBox)
					.WidthOverride(80.f)
					[
						SNew(STextBlock)
						.Visibility_Lambda([LogInfo, this]{
							if (!bShowTag)
								return EVisibility::Collapsed;

							return EVisibility::Visible;
						})
						.Text_Lambda([LogInfo] { return LogInfo->GetTag(); })
						.Font(FSlateFontInfo(Cast<UObject>(MonoFont), 10))
					]
				]

                // FileName
                + SHorizontalBox::Slot().AutoWidth()
                [
                    SNew(SBox)
                    [
                        SNew(STextBlock)
                        .Visibility_Lambda([LogInfo, this]{
                            if (!bShowFileName)
                                return EVisibility::Collapsed;

                            return EVisibility::Visible;
                        })
                        .Text_Lambda([LogInfo] { return LogInfo->GetFileNameWithLine(); })
                        .Font(FSlateFontInfo(Cast<UObject>(MonoFont), 10))
                    ]
                ]

                // ": "
                + SHorizontalBox::Slot().AutoWidth()
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(": "))
                    .Font(FSlateFontInfo(Cast<UObject>(MonoFont), 10))
                ]

                // Primary Message
                + SHorizontalBox::Slot().AutoWidth()
                [
                    SNew(STextBlock)
                    .Text_Lambda([LogInfo] { return LogInfo->GetPrimaryLineMessage(); })
                    .Font(FSlateFontInfo(Cast<UObject>(MonoFont), 10))
                ]

                // " ..."
                + SHorizontalBox::Slot().AutoWidth()
                [
                    SNew(STextBlock)
                    .Visibility_Lambda([LogInfo, this]{
                        if (LogInfo->OpenedNL())
                            return EVisibility::Collapsed;

                        if (!LogInfo->HaveNL())
                            return EVisibility::Collapsed;

                        return EVisibility::Visible;
                    })
                    .Text(FText::FromString(" ..."))
                    .Font(FSlateFontInfo(Cast<UObject>(MonoFont), 10))
                ]
            ]

            // New line
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SBox)
                .Visibility_Lambda([LogInfo]{
                    if (!LogInfo->OpenedNL())
                        return EVisibility::Collapsed;

                    return EVisibility::Visible;
                })
                [
                    SNew(SVerticalBox)

                    // New Line Message
                    + SVerticalBox::Slot().AutoHeight()
                    [
                        SNew(STextBlock)
                        .Visibility_Lambda([LogInfo]{
                            if (!LogInfo->GetNewLineMessage().IsSet())
                                return EVisibility::Collapsed;

                            return EVisibility::Visible;
                        })
                        .Text_Lambda([LogInfo] { return *LogInfo->GetNewLineMessage(); })
                        .Font(FSlateFontInfo(Cast<UObject>(MonoFont), 10))
                    ]

                    // Stack Trace Message
                    + SVerticalBox::Slot().AutoHeight()
                    [
                        SNew(STextBlock)
                        .Visibility_Lambda([LogInfo]{
                            if (!LogInfo->GetNewLineStackTrace().IsSet())
                                return EVisibility::Collapsed;

                            return EVisibility::Visible;
                        })
                        .Text_Lambda([LogInfo] { return *LogInfo->GetNewLineStackTrace(); })
                        .Font(FSlateFontInfo(Cast<UObject>(MonoFont), 10))
                    ]
                ]
            ]
        ];
}

TSharedRef<SWidget> SDT_LoggerTabSlate::GenerateMenuContent()
{
    return
        SNew(SBorder)
        .Padding(5)
        .BorderImage(FCoreStyle::Get().GetBrush("Menu.Background"))
        [
            SNew(SVerticalBox)

            // Show net status
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("Show net status:"))
                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                ]

                + SHorizontalBox::Slot()
                .FillWidth(1.f)

                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    SNew(SCheckBox)
                    .IsChecked(bShowNetStatus)
                    .OnCheckStateChanged_Lambda([this](ECheckBoxState CheckBoxState) {
                        bShowNetStatus = (bool)CheckBoxState;
                    })
                ]
            ]

        	// Show tag
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(5)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Show tag:"))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
				]

				+ SHorizontalBox::Slot()
				.FillWidth(1.f)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(5)
				[
					SNew(SCheckBox)
					.IsChecked(bShowTag)
					.OnCheckStateChanged_Lambda([this](ECheckBoxState CheckBoxState) {
						bShowTag = (bool)CheckBoxState;
					})
				]
			]

            // Show file name
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("Show file name:"))
                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                ]

                + SHorizontalBox::Slot()
                .FillWidth(1.f)

                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    SNew(SCheckBox)
                    .IsChecked(bShowFileName)
                    .OnCheckStateChanged_Lambda([this](ECheckBoxState CheckBoxState) {
                        bShowFileName = (bool)CheckBoxState;
                    })
                ]
            ]

            // Collect stacktrace for verbosity
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("Collect stacktrace for:"))
                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                ]

                + SHorizontalBox::Slot()
                .FillWidth(1.f)

                // Display
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    CreateCollectStacktraceForVerbosityCheckBox(ELogVerbosity::Display)
                ]
                // Warning
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    CreateCollectStacktraceForVerbosityCheckBox(ELogVerbosity::Warning)
                ]
                // Error
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    CreateCollectStacktraceForVerbosityCheckBox(ELogVerbosity::Error)
                ]
            ]

            // Show log of verbosity
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("Show log of verbosity:"))
                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                ]

                + SHorizontalBox::Slot()
                .FillWidth(1.f)

                // Display
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    CreateShowVerbosityCheckBox(ELogVerbosity::Display)
                ]
                // Warning
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    CreateShowVerbosityCheckBox(ELogVerbosity::Warning)
                ]
                // Error
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    CreateShowVerbosityCheckBox(ELogVerbosity::Error)
                ]
            ]

        	// Show only tags
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("Tags:"))
                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                ]


            	+ SHorizontalBox::Slot()
				.FillWidth(1.f)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(5)
	            [
		            SNew(SMultiSelectComboBox)
	            ]
            ]
        ];
}

FReply SDT_LoggerTabSlate::OnOpenMenuClicked()
{
    if (MenuAnchor.IsValid())
    {
        MenuAnchor->SetIsOpen(true, true);
    }
    return FReply::Handled();
}

TSharedRef<SWidget> SDT_LoggerTabSlate::CreateCollectStacktraceForVerbosityCheckBox(ELogVerbosity::Type Verbosity)
{
    bool bIsChecked = false;
    if (const auto Logger = UDT_Logger::Get())
    {
        bIsChecked = Logger->IsLogVerbosityWithStackTrace(Verbosity);
    }

    return SNew(SCheckBox)
        .IsChecked(bIsChecked)
        .OnCheckStateChanged_Lambda([Verbosity](ECheckBoxState CheckBoxState) {
            if (const auto Logger = UDT_Logger::Get())
            {
                Logger->UpdateLogVerbosityWithStackTrace(Verbosity, (bool)CheckBoxState);
            }
        })
        .Style(GetCheckBoxStyle(Verbosity));
}

TSharedRef<SWidget> SDT_LoggerTabSlate::CreateShowVerbosityCheckBox(ELogVerbosity::Type Verbosity)
{
    return SNew(SCheckBox)
        .IsChecked(ShownVerbosity[Verbosity])
        .OnCheckStateChanged_Lambda([this, Verbosity](ECheckBoxState CheckBoxState) {
            ShownVerbosity[Verbosity] = !ShownVerbosity[Verbosity];
        })
        .Style(GetCheckBoxStyle(Verbosity));
}

FCheckBoxStyle* SDT_LoggerTabSlate::GetCheckBoxStyle(ELogVerbosity::Type Verbosity)
{
    static TMap<ELogVerbosity::Type, FCheckBoxStyle> CheckBoxStyles;
    if (const auto Style = CheckBoxStyles.Find(Verbosity))
    {
        return Style;
    }

    FCheckBoxStyle NewStyle = FCoreStyle::Get().GetWidgetStyle<FCheckBoxStyle>("Checkbox");

    auto Color = GetColorForVerbosity(Verbosity);
    Color /= 3.f;
    Color.A = 1.f;
    NewStyle.BackgroundImage.TintColor = Color;
    NewStyle.BackgroundHoveredImage.TintColor = Color;
    NewStyle.BackgroundPressedImage.TintColor = Color;

    return &CheckBoxStyles.Add(Verbosity, MoveTempIfPossible(NewStyle));
}
