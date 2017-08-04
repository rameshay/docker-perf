
/* Web Polygraph       http://www.web-polygraph.org/
 * (C) 2003-2014 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#ifndef POLYGRAPH__CLIENT_HTTPPRINTER_H
#define POLYGRAPH__CLIENT_HTTPPRINTER_H


#include <set>

#include "base/UniqId.h"
#include "pgl/PglContainerSym.h"


class ArraySym;
class MimeHeadersCfg;

// Stream for HTTP, handles duplicate headers.
class HttpPrinter: public ofixedstream, public ContainerSym::Visitor {
	public:
		HttpPrinter(char_type *const buffer, const std::streamsize size, const UniqId &xid);

		bool putHeader(const String &header, const bool isUserConfigured = false);
		void putHeaders(const MimeHeadersCfg *const cfg);

		virtual void visit(const SynSym &item);

	private:
		typedef std::set<String> HeadersSet;
		HeadersSet theHeaders; // current HTTP headers
		const UniqId theXactionId; // transaction we generate the headers for

		/* lazily-computed and cached macro values */
		String theTransactionIdImage; // for the transaction_id macro
};

#endif