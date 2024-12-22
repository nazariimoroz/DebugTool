#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Views/STableViewBase.h"
#include "Framework/SlateDelegates.h"

class SMultiSelectComboBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMultiSelectComboBox) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TArray<TSharedPtr<FString>> Options;

	TArray<TSharedPtr<FString>> SelectedOptions;

	FText NewItemText;

	TSharedPtr<SListView<TSharedPtr<FString>>> ListViewWidget;

private:
	TSharedRef<SWidget> GenerateComboContent();

	TSharedRef<ITableRow> OnGenerateRow(
		TSharedPtr<FString> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	);

	void OnCheckboxStateChanged(ECheckBoxState NewState, TSharedPtr<FString> ChangedItem);

	ECheckBoxState IsItemChecked(TSharedPtr<FString> Item) const;

	FReply OnAddNewItem();

	void OnNewItemTextCommitted(const FText& InText, ETextCommit::Type CommitType);
};
