
/* Web Polygraph       http://www.web-polygraph.org/
 * Copyright 2003-2011 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#ifndef POLYGRAPH__RUNTIME_ADDRMAP_H
#define POLYGRAPH__RUNTIME_ADDRMAP_H

#include "xstd/Array.h"

class ArraySym;
class NetAddr;
class NetAddrSym;
class SrcLoc;
class AddrMapSym;
class AddrMap;
class AddrMapItem;

// addr iterator for the map below
class AddrMapAddrIter {
	public:
		AddrMapAddrIter(const AddrMap &aMap, int aName);

		operator void *() const { return atEnd() ? 0 : (void*)-1; }
		AddrMapAddrIter &operator ++() { next(); return *this; }

		const NetAddr &addr();
		const NetAddr &name();

	protected:
		bool atEnd() const;
		void next() { ++theAddr; }

	protected:
		const AddrMap &theMap;
		int theName;
		int theAddr;
};

// name -> address map
// note that find() has to optimize the map if items were added
// so calling add/query often run-time is not a good idea
class AddrMap {
	public:
		typedef Array<NetAddr*> Addrs;
		
	public:
		AddrMap();
		~AddrMap();

		void configure(const Array<AddrMapSym*> &maps);
		void addIp(const SrcLoc &context, const NetAddr &name); // name->name

		int nameCount() const;
		const NetAddr &nameAt(int nameIdx) const;
		const AddrMapItem &itemAt(int nameIdx) const;

		// use iterator instead
		const NetAddr &addrAt(int nameIdx, int addrIdx) const;
		int addrCountAt(int nameIdx) const;

		bool has(const NetAddr &name) const;
		bool find(const NetAddr &name, int &idx) const;

		bool findAddr(const NetAddr &addr) const;
		const NetAddr &selectAddr(int nameIdx) const;

		AddrMapAddrIter addrIter(int nameIdx) const;
		AddrMapAddrIter addrIter(const NetAddr &name) const;

	protected:
		void importMap(const AddrMapSym &ms);
		void import1to1(const NetAddrSym &name, const NetAddr &addr);
		void importRoundRobin(const ArraySym* names, const Addrs &addrs);
		void import1toN(const NetAddrSym &name, const Addrs &addrs);
		void addMap(const SrcLoc &context, AddrMapItem *item);

		bool findExact(const NetAddr &name, int &idx) const;
		bool findWildcard(const NetAddr &name, int &idx) const;

	protected:
		Array<AddrMapItem*> theMap;
		bool hasWildcards; // at least one domain name starts with "*."
		mutable bool isSorted;
};

extern AddrMap *TheAddrMap;

#endif
