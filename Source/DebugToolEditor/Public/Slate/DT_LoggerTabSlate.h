// TTVideoGenerator by Nazarii Moroz

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class DEBUGTOOLEDITOR_API SDT_LoggerTabSlate : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SDT_LoggerTabSlate){}
        SLATE_ARGUMENT(TArray<FString>, ItemList)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    TSharedRef<SWidget> MakeBlueSquareButton(const FString& ButtonLabel)
    {
        return SNew(SButton)
        .ButtonColorAndOpacity(FLinearColor::Blue)
        .ContentPadding(FMargin(8))
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        .OnClicked(this, &SDT_LoggerTabSlate::OnBlueButtonClicked, ButtonLabel)
        [
            SNew(STextBlock)
            .Text(FText::FromString(ButtonLabel))
            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
            .ColorAndOpacity(FLinearColor::White)
        ];
    }

    FReply OnRefreshClicked()
    {
        // Handle refresh logic here
        return FReply::Handled();
    }

    FReply OnBlueButtonClicked(FString ButtonLabel)
    {
        // Handle blue button logic here
        return FReply::Handled();
    }

private:

    TSharedRef<ITableRow> OnGenerateRowForList(
        TSharedPtr<FString> Item,
        const TSharedRef<STableViewBase>& OwnerTable
    ) const;
};
