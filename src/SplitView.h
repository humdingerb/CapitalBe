/*
 * Copyright 2009-2024. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *	darkwyrm (Jon Yoder)
 *	humdinger (Joachim Seemer)
 *	raefaldhia (Raefaldhi Amartya Junior)
 */
#ifndef SPLITVIEW_H
#define SPLITVIEW_H

#include <Button.h>
#include <CheckBox.h>
#include <List.h>
#include <ListView.h>
#include <ScrollView.h>
#include <StringView.h>
#include <TextControl.h>
#include <View.h>

#include "CategoryButton.h"
#include "Fixed.h"
#include "Notifier.h"
#include "TransactionData.h"

// clang-format off
enum {
	M_ADD_SPLIT = 'mads',
	M_REMOVE_SPLIT,
	M_ENTER_TRANSACTION = 'metr',
	M_EXPANDER_CHANGED,
	M_SELECT_SPLIT,
	M_SPLIT_CATEGORY_CHANGED,
	M_SPLIT_AMOUNT_CHANGED,
	M_SPLIT_MEMO_CHANGED,
	M_EDIT_KEY,
	M_NEXT_SPLIT,
	M_PREVIOUS_SPLIT,
	M_SHOW_CALENDER,
	M_SET_DATE
};
// clang-format on

class SplitViewFilter;
class Category;
class DateBox;
class PayeeBox;
class CurrencyBox;
class CategoryBox;
class NavTextBox;
class HelpButton;

class SplitView : public BView, public Observer {
public:
	SplitView(const char* name, const TransactionData& trans, const int32& flags);
	~SplitView();

	void AttachedToWindow();
	void DetachedFromWindow();
	void MessageReceived(BMessage* msg);

	void SetFields(const char* date, const char* type, const char* payee, const char* amount,
		const char* category, const char* memo);

	void HandleNotify(const uint64& value, const BMessage* msg);
	void MakeEmpty();
	void MakeFocus(bool value = true);
	void FrameResized(float width, float height);

	bool IsSplitHidden() const { return fSplitContainer->IsHidden(); }
	void ToggleSplit();

private:
	friend SplitViewFilter;
	bool _ValidateAllFields();
	//	bool ValidateDateField();
	//	bool ValidateAmountField();
	bool _ValidateSplitAmountField();
	//	bool ValidateCategoryField();
	bool _ValidateSplitItems();
	Fixed _CalculateTotal();
	Category* _MakeCategory();

	DateBox* fDate;
	PayeeBox* fPayee;
	CurrencyBox* fAmount;
	CategoryBox* fCategory;
	CategoryButton* fCategoryButton;
	NavTextBox* fMemo;
	SplitViewFilter* fKeyFilter;
	BMessenger* fMessenger;
	BButton *fEnter, *fTransfer;

	BCheckBox* fSplit;

	BView* fSplitContainer;
	CategoryBox* fSplitCategory;
	BTextControl *fSplitAmount, *fSplitMemo;
	BListView* fSplitItems;
	BScrollView* fSplitScroller;
	BButton *fAddSplit, *fRemoveSplit;
	BStringView* fSplitTotal;
	BCheckBox* fReconciled;

	time_t fCurrentDate;
	bool fStartExpanded;
	TransactionData fTransaction;
};

#endif	// SPLITVIEW_H
