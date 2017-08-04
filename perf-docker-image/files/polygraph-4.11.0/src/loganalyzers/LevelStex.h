
/* Web Polygraph       http://www.web-polygraph.org/
 * Copyright 2003-2011 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#ifndef POLYGRAPH__LOGANALYZERS_LEVELSTEX_H
#define POLYGRAPH__LOGANALYZERS_LEVELSTEX_H

#include "xstd/String.h"
#include "base/StatIntvlRec.h"
#include "base/LevelStat.h"

// an algorithm of extracting a particular level statistics out of
// interval stats record
class LevelStex {
	public:
		// TODO: Replace with lambdas
		typedef LevelStat StatIntvlRec::*StatPtr;
		typedef ProtoIntvlStat StatIntvlRec::*ProtoPtr;
		typedef const LevelStat &(ProtoIntvlStat::*LevelMethod)() const;

	public:
		LevelStex(const String &aKey, const String &aName, StatPtr aStats):
			theKey(aKey), theName(aName), theStats(aStats) {}
		LevelStex(const String &aKey, const String &aName, ProtoPtr aProto, LevelMethod aMethod):
			theKey(aKey), theName(aName), theStats(0), theProto(aProto), theLevelMethod(aMethod) {}

		const String &key() const { return theKey; } // precise, for machine use
		const String &name() const { return theName; } // imprecise, human-readable

		const LevelStat &level(const StatIntvlRec &rec) const {
			return theStats ? rec.*theStats : (rec.*theProto.*theLevelMethod)();
		}

	protected:
		String theKey;
		String theName;

		// Alternative A: Extract LevelStat directly from a trace window.
		StatPtr theStats;
		// Alternative B: First extract ProtoIntvlStat from a trace window, and
		// then LevelStat from the extracted ProtoIntvlStat.
		ProtoPtr theProto;
		LevelMethod theLevelMethod;
};

#endif
