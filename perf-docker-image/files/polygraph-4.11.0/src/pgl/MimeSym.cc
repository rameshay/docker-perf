
/* Web Polygraph       http://www.web-polygraph.org/
 * Copyright 2003-2011 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#include "pgl/pgl.h"

#include "xstd/String.h"
#include "pgl/PglRec.h"
#include "pgl/PglStringSym.h"
#include "pgl/PglArraySym.h"
#include "pgl/MimeSym.h"



String MimeSym::TheType = "Mime";

static String strExtensions = "extensions";
static String strPrefixes = "prefixes";
static String strQueries = "queries";
static String strStringArr = "string[]";
static String strType = "type";


MimeSym::MimeSym(): RecSym(TheType, new PglRec) {
	theRec->bAdd(StringSym::TheType, strType, 0);

	// XXX: move to UrlGen
	theRec->bAdd(strStringArr, strPrefixes, 0);
	theRec->bAdd(strStringArr, strExtensions, 0);
	theRec->bAdd(strStringArr, strQueries, 0);
}

MimeSym::MimeSym(const String &aType, PglRec *aRec): RecSym(aType, aRec) {
}

bool MimeSym::isA(const String &type) const {
	return RecSym::isA(type) || type == TheType;
}

SynSym *MimeSym::dupe(const String &type) const {
	if (isA(type))
		return new MimeSym(this->type(), theRec->clone());
	return RecSym::dupe(type);
}

String MimeSym::mimeType() const {
	return getString(strType);
}

bool MimeSym::field(Array<String*> &values, RndDistr *&selector, const String &name) const {
	if (ArraySym *as = getArraySym(name)) {
		selector = as->makeSelector(TheType + '-' + name);
		Assert(getStrings(name, values));
		return true;
	}
	return false;
}

bool MimeSym::extensions(Array<String*> &exts, RndDistr *&selector) const {
	return field(exts, selector, strExtensions);
}

bool MimeSym::prefixes(Array<String*> &pxs, RndDistr *&selector) const {
	return field(pxs, selector, strPrefixes);
}

bool MimeSym::queries(Array<String*> &qry, RndDistr *&selector) const {
	return field(qry, selector, strQueries);
}
