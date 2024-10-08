/*
 * Copyright 2009-2024. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *	darkwyrm (Jon Yoder)
 *	raefaldhia (Raefaldhi Amartya Junior)
 */
#ifndef NUMBOX_H
#define NUMBOX_H

#include "AutoTextControl.h"

class NumBox;

class NumBoxFilter : public AutoTextControlFilter {
public:
	NumBoxFilter(NumBox* box);
	filter_result KeyFilter(const int32& key, const int32& mod);
};

class NumBox : public AutoTextControl {
public:
	NumBox(const char* name, const char* label, const char* text, BMessage* msg,
		uint32 flags = B_WILL_DRAW | B_NAVIGABLE);

	bool Validate(bool alert = true);

	void UseTabFiltering(const bool& value) { fFilterTab = value; }
	bool IsTabFiltering() const { return fFilterTab; }

	void AllowNegatives(const bool& value);
	bool AllowsNegatives() const { return fAllowNegatives; }

private:
	friend NumBoxFilter;
	bool fFilterTab;
	bool fAllowNegatives;
};

#endif	// NUMBOX_H
