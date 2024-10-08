/*
 * Copyright 2009-2024. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *	darkwyrm (Jon Yoder)
 *	dospuntos (Johan Wagenheim)
 *	humdinger (Joachim Seemer)
 */
#ifndef RECONCILEWINDOW_H
#define RECONCILEWINDOW_H

#include <Button.h>
#include <ListItem.h>
#include <ListView.h>
#include <Message.h>
#include <StringView.h>
#include <TextControl.h>
#include <View.h>
#include <Window.h>

#include "Account.h"
#include "Help.h"
#include "Notifier.h"

class DateBox;
class CurrencyBox;
class ReconcileItem;

void _AddReconcileItems(const TransactionData& data, void* recwin);

class ReconcileWindow : public BWindow, public Observer {
public:
	ReconcileWindow(const BRect frame, Account* account);
	~ReconcileWindow();

	void MessageReceived(BMessage* msg);

	void HandleNotify(const uint64& value, const BMessage* msg);
	bool QuitRequested();

private:
	friend class _ReconcileFilter;
	friend void _AddReconcileItems(const TransactionData& data, void* ptr);

	void _ApplyChargesAndInterest();
	bool _AutoReconcile();
	ReconcileItem* _FindItemForID(BListView* target, const uint32& id);
	void _InsertTransactionItem(BListView* target, ReconcileItem* item);

	Account* fAccount;

	BListView *fDepositList, *fChargeList;
	CurrencyBox *fOpening, *fClosing, *fCharges, *fInterest;
	DateBox* fDate;
	BButton *fReset, *fReconcile, *fCancel, *fAutoReconcile;

	// fTotal is the sum of all of the deposits, checks, and charges
	// The way to tell if we are done is if fDifference + fTotal == 0
	Fixed fDepositTotal, fChargeTotal, fDifference, fTotal;
	BStringView *fDepLabel, *fChargeLabel, *fTotalLabel, *fDateLabel, *fChargesLabel;
	BScrollView *fDepScroll, *fChargeScroll;

	HelpButton* fHelpButton;

	float fDateMultiplier, fOpeningMultiplier, fClosingMultiplier;
	time_t fCurrentDate;
};

#endif	// RECONCILEWINDOW_H
