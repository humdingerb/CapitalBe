/*
	AutoTextControl.h: A BTextControl which notifies on each keypress
	Written by DarkWyrm <darkwyrm@earthlink.net>, Copyright 2007
	Released under the MIT license.
*/
#ifndef AUTO_TEXT_CONTROL_H
#define AUTO_TEXT_CONTROL_H

#include <MessageFilter.h>
#include <TextControl.h>

class AutoTextControlFilter;

/*
	The AutoTextControl provides realtime updates to any changes made to it.
	It also provides the ability to limit the text to a certain number of
	characters.

	If, for some reason, you want to disable the updates-per-keypress, pass
	a regular BMessageFilter to the SetFilter method.
*/

class AutoTextControl : public BTextControl {
public:
	AutoTextControl(const char* name, const char* label, const char* text, BMessage* msg,
		uint32 flags = B_WILL_DRAW | B_NAVIGABLE);

	AutoTextControl(BMessage* data);
	static BArchivable* Instantiate(BMessage* data);
	virtual status_t Archive(BMessage* data, bool deep = true) const;

	virtual status_t GetSupportedSuites(BMessage* msg);
	virtual BHandler* ResolveSpecifier(
		BMessage* msg, int32 index, BMessage* specifier, int32 form, const char* property);

	virtual ~AutoTextControl();

	virtual void AttachedToWindow();
	virtual void DetachedFromWindow();

	void SetFilter(AutoTextControlFilter* filter);

	AutoTextControlFilter* GetFilter() { return fFilter; }

	void SetCharacterLimit(const uint32& limit);
	uint32 GetCharacterLimit(const uint32& limit);

private:
	friend class AutoTextControlFilter;

	AutoTextControlFilter* fFilter;
	uint32 fCharLimit;
};

/*
	This class does all of the heavy lifting for AutoTextControl's realtime
	updates.

	You can further customize input and updates by subclassing the
	KeyFilter hook function. When doing so, the current key message can
	be accessed by way of GetCurrentMessage(). However, it will return NULL
	when called from any other method.
*/
class AutoTextControlFilter : public BMessageFilter {
public:
	AutoTextControlFilter(AutoTextControl* checkview);
	~AutoTextControlFilter();
	virtual filter_result Filter(BMessage* msg, BHandler** target);
	virtual filter_result KeyFilter(const int32& key, const int32& mod);

	AutoTextControl* TextControl() const { return fBox; }

	BMessage* GetCurrentMessage() { return fCurrentMessage; }

	void SendMessage(BMessage* msg);
	void SetMessenger(BMessenger* msgr);
	BMessenger* GetMessenger();

private:
	AutoTextControl* fBox;
	BMessage* fCurrentMessage;
	BMessenger* fMessenger;
};

#endif	// AUTO_TEXT_CONTROL_H
