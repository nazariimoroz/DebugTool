#include "Slate/SMultiSelectComboBox.h"

void SMultiSelectComboBox::Construct(const FArguments& InArgs)
{
    Options.Add(MakeShared<FString>(TEXT("Option A")));
    Options.Add(MakeShared<FString>(TEXT("Option B")));
    Options.Add(MakeShared<FString>(TEXT("Option C")));
    Options.Add(MakeShared<FString>(TEXT("Option D")));

    ChildSlot
    [
        SNew(SComboButton)
        .ButtonContent()
        [
            SNew(STextBlock)
            .Text(FText::FromString(TEXT("Select Options...")))
        ]
        .OnGetMenuContent(this, &SMultiSelectComboBox::GenerateComboContent)
    ];
}

TSharedRef<SWidget> SMultiSelectComboBox::GenerateComboContent()
{
    if (!ListViewWidget.IsValid())
    {
        SAssignNew(ListViewWidget, SListView<TSharedPtr<FString>>)
        .ListItemsSource(&Options)
        .OnGenerateRow(this, &SMultiSelectComboBox::OnGenerateRow)
        .SelectionMode(ESelectionMode::None)
        .IsFocusable(true);
    }

    return SNew(SVerticalBox)
    + SVerticalBox::Slot()
    .MaxHeight(300.0f)
    .AutoHeight()
    [
        SNew(SScrollBox)
        + SScrollBox::Slot()
        [
            ListViewWidget.ToSharedRef()
        ]
    ]
    + SVerticalBox::Slot()
    .AutoHeight()
    .Padding(5.0f)
    [
        SNew(SHorizontalBox)

        + SHorizontalBox::Slot()
        .FillWidth(1.0f)
        [
            SNew(SEditableTextBox)
            .Text(NewItemText)
            .OnTextCommitted(this, &SMultiSelectComboBox::OnNewItemTextCommitted)
            .HintText(FText::FromString(TEXT("Add new item...")))
        ]

        + SHorizontalBox::Slot()
        .AutoWidth()
        .Padding(FMargin(5.0f, 0.f))
        [
            SNew(SButton)
            .Text(FText::FromString(TEXT("Add")))
            .OnClicked(this, &SMultiSelectComboBox::OnAddNewItem)
        ]
    ];
}

TSharedRef<ITableRow> SMultiSelectComboBox::OnGenerateRow(
    TSharedPtr<FString> InItem,
    const TSharedRef<STableViewBase>& OwnerTable)
{
    return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
    [
        SNew(SCheckBox)
        .OnCheckStateChanged(this, &SMultiSelectComboBox::OnCheckboxStateChanged, InItem)
        .IsChecked(this, &SMultiSelectComboBox::IsItemChecked, InItem)
        [
            SNew(STextBlock)
            .Text(FText::FromString(*InItem))
        ]
    ];
}

void SMultiSelectComboBox::OnCheckboxStateChanged(ECheckBoxState NewState, TSharedPtr<FString> ChangedItem)
{
    if (!ChangedItem.IsValid())
    {
        return;
    }

    if (NewState == ECheckBoxState::Checked)
    {
        if (!SelectedOptions.Contains(ChangedItem))
        {
            SelectedOptions.Add(ChangedItem);
        }
    }
    else
    {
        SelectedOptions.Remove(ChangedItem);
    }
}

ECheckBoxState SMultiSelectComboBox::IsItemChecked(TSharedPtr<FString> Item) const
{
    if (SelectedOptions.Contains(Item))
    {
        return ECheckBoxState::Checked;
    }
    return ECheckBoxState::Unchecked;
}

FReply SMultiSelectComboBox::OnAddNewItem()
{
    FString TrimmedText = FText::TrimPrecedingAndTrailing(NewItemText).ToString();
    if (!TrimmedText.IsEmpty())
    {
        TSharedPtr<FString> NewOption = MakeShared<FString>(TrimmedText);
        Options.Add(NewOption);

        SelectedOptions.Add(NewOption);

        if (ListViewWidget.IsValid())
        {
            ListViewWidget->RequestListRefresh();
        }
    }

    NewItemText = FText::GetEmpty();
    return FReply::Handled();
}

void SMultiSelectComboBox::OnNewItemTextCommitted(const FText& InText, ETextCommit::Type CommitType)
{
	NewItemText = InText;
    if (CommitType == ETextCommit::OnEnter)
    {
        OnAddNewItem();
    }
}
