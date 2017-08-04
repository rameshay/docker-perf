
/* Web Polygraph       http://www.web-polygraph.org/
 * Copyright 2003-2016 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#ifndef POLYGRAPH__RUNTIME_SSLERROR_H
#define POLYGRAPH__RUNTIME_SSLERROR_H

class Error;
class SrcLoc;

namespace SslErrors {
// Prints and clears the global error stack.
// Does nothing if the stack is already empty.
std::ostream &Print(std::ostream &os);

// If an error report is needed, calls Print() and returns true.
// Otherwise, clears the global stack and returns false.
bool Report(std::ostream &os, const SrcLoc &loc, const Error &e, int logCat);
}

#endif
