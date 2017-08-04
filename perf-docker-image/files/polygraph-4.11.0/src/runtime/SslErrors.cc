
/* Web Polygraph       http://www.web-polygraph.org/
 * Copyright 2003-2016 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#include "base/polygraph.h"

#include "xstd/String.h"
#include "xstd/Ssl.h"
#include "runtime/ErrorMgr.h"
#include "runtime/LogComment.h"
#include "runtime/SslErrors.h"


std::ostream &SslErrors::Print(std::ostream &os) {
	const char *fname;
	int line;
	bool printedHeader = false;
	while (const unsigned long e = SslMisc::ErrGetErrorLine(&fname, &line)) {
		if (!printedHeader) {
			os << "SSL error stack:" << std::endl;
			printedHeader = true;
		} else {
			os << std::endl;
		}
		os << "\t* " << fname << ":" << line << ": " <<
			SslMisc::ErrErrorString(e);
	}
	if (printedHeader)
		os << endc;

	return os;
}

bool SslErrors::Report(std::ostream &os, const SrcLoc &loc, const Error &e, int logCat) {
    if (TheErrorMgr.reportError(loc, e, logCat)) {
        Print(os);
        return true;
    }
    SslMisc::ErrClearError();
    return false;
}
