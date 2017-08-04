
/* Web Polygraph       http://www.web-polygraph.org/
 * Copyright 2003-2011 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#include "base/polygraph.h"

#include "xstd/String.h"

#include "base/macros.h"

// All macros must begin with '$' or '%'!
// TODO: Reject (invalid) escape sequences.
// TODO: Support escape sequence to protect non-macros.

// Returns the start of a macro or npos if `str` has no macros.
// The start may not correspond to the supplied `macro` name,
// which is only used to determine the macro opening character.
static
int MacroStart(const String &str, const String &macro) {
	if (!macro)
		return String::npos;

	const char opening = macro[0];
	Assert(opening == '$' || opening == '%');
	return str.find(opening, 0);
}

// Determines whether `str` has any macros.
// The supplied `macro` name is only used to determine the macro opening character.
bool HasSomeMacros(const String &str, const String &macro) {
	return MacroStart(str, macro) != String::npos;
}

// Simple macro expansion.
// Replaces all occurences of @macro@ in @str@ with @replacement@ and returns the
// resulting string.
String ExpandMacro(const String &str, const String &macro, const String &replacement) {
	int searchStart = MacroStart(str, macro);
	if (searchStart == String::npos)
		return str; // no macros at all

	/* there are macros, but not necessarily our macro */

	String result;

	int headStart = 0; // we searched but did not process the head yet

	const Area m(Area::Create(macro.cstr()));
	while (searchStart < str.len()) {
		const int pos = str.find(m, searchStart);
		if (pos == String::npos)
			break;
		result += str(headStart, pos); // head
		result += replacement;
		searchStart = headStart = pos + m.size();
	}

	if (!headStart)
		return str; // found some macro(s), but not our macro

	if (headStart < str.len())
		result += str(headStart, str.len());  // tail

	return result;
}
