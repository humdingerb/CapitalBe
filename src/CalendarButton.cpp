/*
 * Copyright 2024. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *	humdinger (Joachim Seemer)
 */
#include "CalendarButton.h"
#include "CalendarMenuWindow.h"

#include <Bitmap.h>
#include <DateFormat.h>
#include <Picture.h>

// clang-format off
enum {
	M_SHOW_CALENDER,
	M_SET_DATE
};
// clang-format on

CalendarButton::CalendarButton(DateBox* datebox)
	: BButton("calenderbutton", "", new BMessage(M_SHOW_CALENDER)),
	  fDateBox(datebox)
{
	float height;
	fDateBox->GetPreferredSize(NULL, &height);
	BSize size(height - 2, height);
	SetExplicitSize(size);

	_UpdateIcon();
}


void
CalendarButton::AttachedToWindow()
{
	SetTarget(this);
}


void
CalendarButton::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case M_SHOW_CALENDER:
		{
			_ShowPopUpCalendar();
			break;
		}
		case M_SET_DATE:
		{
			int32 day, month, year;
			BString date;
			msg->FindInt32("day", &day);
			msg->FindInt32("month", &month);
			msg->FindInt32("year", &year);
			date << day << "." << month << "." << year;
			fDateBox->SetText(date);
			fDateBox->Invoke();
			fDateBox->Validate();

			_UpdateIcon();
			break;
		}
		default:
		{
			BButton::MessageReceived(msg);
		}
	}
}


BBitmap*
CalendarButton::_DrawIcon()
{
	font_height fh;
	GetFontHeight(&fh);
	float fontHeight = floorf(fh.ascent + fh.descent + fh.leading);
	BRect rect(0, 0, fontHeight - 2, fontHeight);

	BView* view = new BView(rect, NULL, B_FOLLOW_ALL, B_WILL_DRAW);
	BBitmap* bitmap = new BBitmap(rect, B_RGBA32, true);
	bitmap->Lock();
	bitmap->AddChild(view);

	view->SetDrawingMode(B_OP_COPY);
	view->SetHighUIColor(B_PANEL_BACKGROUND_COLOR);
	view->SetLowUIColor(B_PANEL_BACKGROUND_COLOR);

	view->FillRect(rect);
	view->SetHighUIColor(B_CONTROL_TEXT_COLOR);
	view->SetPenSize(1.0);
	view->StrokeRect(rect);

	rect.bottom = rect.Height() / 3;
	view->FillRect(rect);

	BFont font(be_plain_font);
	font.SetSize(font.Size() * 0.7);
	view->SetFont(&font, B_FONT_SIZE);

	float dateWidth = view->StringWidth(fDay);
	rect = view->Bounds();
	view->DrawString(fDay.String(),
		BPoint(ceilf((rect.Width() / 2) - dateWidth / 2), ceilf(rect.bottom - rect.Height() / 8)));

	view->RemoveSelf();
	bitmap->Unlock();
	delete view;

	return bitmap;
}


void
CalendarButton::_UpdateIcon()
{
	fDay = "";
	BDateTime now = BDateTime::CurrentDateTime(B_LOCAL_TIME);
	BDate date = now.Time_t();
	fDay << date.Day();

	SetIcon(_DrawIcon());
}


void
CalendarButton::_ShowPopUpCalendar()
{
	if (fCalendarWindow.IsValid()) {
		BMessage activate(B_SET_PROPERTY);
		activate.AddSpecifier("Active");
		activate.AddBool("data", true);

		if (fCalendarWindow.SendMessage(&activate) == B_OK)
			return;
	}

	BDate date = fDateBox->GetDate();
	BMessage* invocationMessage = new BMessage(M_SET_DATE);

	// Get the center of the BButton
	BRect buttonRect(Bounds());
	BPoint where = buttonRect.LeftTop();
	where.x += buttonRect.Width() / 2;
	where.y += buttonRect.Height() / 2;
	ConvertToScreen(&where);

	CalendarMenuWindow* window = new CalendarMenuWindow(this, where);
	window->SetDate(date);
	window->SetInvocationMessage(invocationMessage);
	window->SetDate(date);
	fCalendarWindow = BMessenger(window);
	window->Show();
	window->MoveOnScreen(B_MOVE_IF_PARTIALLY_OFFSCREEN);
}
