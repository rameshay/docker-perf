
/* Web Polygraph       http://www.web-polygraph.org/
 * (C) 2003-2014 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#ifndef POLYGRAPH__PGL_MIMEHEADERSYM_H
#define POLYGRAPH__PGL_MIMEHEADERSYM_H

#include "xstd/String.h"
#include "pgl/PglRecSym.h"

class MimeHeaderSym: public RecSym {
	public:
		static const String TheType;

		static const String TransactionIdMacro;

	public:
		MimeHeaderSym();
		MimeHeaderSym(const String &aType, PglRec *aRec);

		static MimeHeaderSym *Parse(const String &s);

		virtual bool isA(const String &type) const;

		const String *name() const;
		const String *value() const;

		bool hasMacros() const; // assumes no future value() changes!

	protected:
		virtual SynSym *dupe(const String &dType) const;

	private:
		/* on-demand computed and cached hasMacros() answer */
		mutable bool checkedMacros_; // whether hasMacros_ has been computed
		mutable bool hasMacros_; // whether the value contains at least one ${macro}
};

#endif
