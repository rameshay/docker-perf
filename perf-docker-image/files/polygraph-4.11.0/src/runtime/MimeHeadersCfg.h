
/* Web Polygraph       http://www.web-polygraph.org/
 * (C) 2003-2014 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#ifndef POLYGRAPH__CLIENT_MIMEHEADERSCFG_H
#define POLYGRAPH__CLIENT_MIMEHEADERSCFG_H


#include "pgl/pgl.h"
#include "pgl/PglContainerSym.h"


class ArraySym;


// Manages MIME headers array configuration.
class MimeHeadersCfg: public ContainerSym::Visitor {
	public:
		MimeHeadersCfg(const ArraySym &sym);

		void selectHeaders(Visitor &v) const;

		virtual void visit(const SynSym &item);

		bool hasMacros() const { return hasMacros_; }

		static bool Clashes() { return SawClash; }

	private:
		const ArraySym *const theSym;

		/* cached hasMacros() answer */
		bool checkedMacros_; // whether hasMacros_ has been computed
		bool hasMacros_; // whether at least one ${macro} is present in values

		// whether Polygraph generates some user-configured header(s)
		static bool SawClash;
};

#endif
