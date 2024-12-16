// TTVideoGenerator by Nazarii Moroz

#include "Slate/DT_LoggerTabSlate.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Views/SListView.h"
#include "Styling/CoreStyle.h"

void SDT_LoggerTabSlate::Construct(const FArguments& InArgs)
{
    ChildSlot[SNew(SBorder).Padding(FMargin(
        10))[SNew(SVerticalBox)

                // TOP BAR
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SNew(SHorizontalBox)

                    // Left Refresh Button
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(5)
                    [
                        SNew(SButton)

                        .ButtonStyle(&FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"))
                        .ContentPadding(FMargin(10, 5))
                        .OnClicked(this, &SDT_LoggerTabSlate::OnRefreshClicked)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString("REFRESH"))
                            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
                        ]
                    ]

                    // Some Blue Square Buttons
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(5)
                    [
                        MakeBlueSquareButton("B1")
                    ]

                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(5)
                    [
                        MakeBlueSquareButton("B2")
                    ]

                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(5)
                    [
                        MakeBlueSquareButton("B3")
                    ]

                    // Font label and red input field
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .VAlign(VAlign_Center)
                    .Padding(15,0)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString("Font:"))
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                    ]

                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(5)
                    [
                        SNew(SEditableTextBox)
                        .Text(FText::FromString("Enter Font..."))
                        .MinDesiredWidth(100)
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                    ]

                    // File label and red input field
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .VAlign(VAlign_Center)
                    .Padding(15,0)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString("File:"))
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                    ]

                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(5)
                    [
                        SNew(SEditableTextBox)
                        .Text(FText::FromString("Enter Filename..."))
                        .MinDesiredWidth(100)
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
                    ]

                    // Right Refresh Button
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(5)
                    [
                        SNew(SButton)
                        .ButtonStyle(&FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"))
                        .ContentPadding(FMargin(10, 5))
                        .OnClicked(this, &SDT_LoggerTabSlate::OnRefreshClicked)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString("REFRESH"))
                            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
                        ]
                    ]
                ]

                // SPACER to separate top bar and list
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5)
                [
                    SNew(SSeparator)
                ]

                // MAIN LIST AREA
                + SVerticalBox::Slot()
                .FillHeight(1.0f)
                .Padding(5)
                [
                    SNew(SBorder)
                    .BorderImage(FCoreStyle::Get().GetBrush("GenericBlackBox"))
                    [
                        SNew(SScrollBox)
                        + SScrollBox::Slot()
                        [
                            SNew(SVerticalBox)
                            // Populate list items
                            + SVerticalBox::Slot()
                            .AutoHeight()
                        ]
                    ]
                ]
            ]
        ];
}
TSharedRef<ITableRow> SDT_LoggerTabSlate::OnGenerateRowForList(TSharedPtr<FString> Item,
    const TSharedRef<STableViewBase>& OwnerTable) const
{
    return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
    [
        SNew(STextBlock)
        .Text(FText::FromString(*Item))
        .ColorAndOpacity(FLinearColor::Black)
        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
    ];
}