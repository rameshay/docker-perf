
/* Web Polygraph       http://www.web-polygraph.org/
 * Copyright 2003-2011 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#include "base/polygraph.h"

#include <fstream>

#include "xml/XmlAttr.h"
#include "loganalyzers/LevelStex.h"
#include "loganalyzers/ReportBlob.h"
#include "loganalyzers/BlobDb.h"
#include "loganalyzers/RepOpts.h"
#include "loganalyzers/PhaseInfo.h"
#include "loganalyzers/PhaseTrace.h"
#include "loganalyzers/LevelTraceFig.h"


LevelTraceFig::LevelTraceFig(): theStex(0), thePhase(0), theTrace(0) {
	theAxisY1.label("level, #");
}

void LevelTraceFig::stats(const LevelStex *const aStex, const PhaseInfo *const aPhase) {
	theStex = aStex;
	thePhase = aPhase;
	theTrace = &thePhase->trace();
	Assert(theTrace);
}

void LevelTraceFig::compareWith(const LevelStex *const stex) {
	if (hasDataLines(stex))
		theComparison.append(stex);
}

int LevelTraceFig::addPlotData() {
	// make sure that the most interesting line is on top
	theComparison.append(theStex);

	// create plot command
	for (int i = 0; i < theComparison.count(); ++i)
		addPlotLine(theComparison[i]->name());

	int pointCount = 0;
	for (int s = 0; s < theComparison.count(); ++s)
		pointCount += dumpDataLines(theComparison[s]);

	return pointCount;
}

int LevelTraceFig::dumpDataLines(const LevelStex *stex) {
	int pointCount = 0;
	for (int i = 0; i < theTrace->count(); ++i)
		pointCount += dumpDataLine(stex, theTrace->winPos(i), theTrace->winStats(i));
	addedLineData();
	return pointCount;
}

int LevelTraceFig::dumpDataLine(const LevelStex *stex, Time stamp, const StatIntvlRec &r) {
	const LevelStat &stat = stex->level(r);
	if (stat.known()) {
		addDataPoint(stamp, stat.level()); // XXX: Why does not stat.mean() work here?
		return 1;
	}
	return 0;	
}

// TODO: Avoid PointTraceFig code duplication, including this method.
bool LevelTraceFig::hasDataLines(const LevelStex *stex) const {
	for (int i = 0; i < theTrace->count(); ++i) {
		const StatIntvlRec &r = theTrace->winStats(i);
		if (stex->level(r).known())
			return true;
	}
	return false;
}
