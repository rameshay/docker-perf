
/* Web Polygraph       http://www.web-polygraph.org/
 * Copyright 2003-2011 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#include "pgl/pgl.h"

#include "xstd/String.h"
#include "pgl/AddrMapSym.h"
#include "pgl/PglArraySym.h"
#include "pgl/PglNetAddrSym.h"
#include "pgl/PglRec.h"
#include "pgl/PglSemx.h"
#include "pgl/PglStringSym.h"


String AddrMapSym::TheType = "AddrMap";

static String strAddrArr = "addr[]";
static String strAddresses = "addresses";
static String strNames = "names";
static String strZone = "zone";


AddrMapSym::AddrMapSym(): RecSym(TheType, new PglRec) {
	theRec->bAdd(StringSym::TheType, strZone, 0);
	theRec->bAdd(strAddrArr, strAddresses, 0);
	theRec->bAdd(strAddrArr, strNames, 0);
}

AddrMapSym::AddrMapSym(const String &aType, PglRec *aRec): RecSym(aType, aRec) {
}

bool AddrMapSym::isA(const String &type) const {
	return RecSym::isA(type) || type == TheType;
}

SynSym *AddrMapSym::dupe(const String &type) const {
	if (isA(type))
		return new AddrMapSym(this->type(), theRec->clone());
	return RecSym::dupe(type);
}

bool AddrMapSym::usable() const {
	const ArraySym *addrs = addressesSym();
	const ArraySym *names = namesSym();
	return addrs && names && !addrs->empty() && !names->empty();
}

String AddrMapSym::zone() const {
	return getString(strZone);
}

bool AddrMapSym::addresses(Array<NetAddr*> &addrs) const {
	return getNetAddrs(strAddresses, addrs);
}

bool AddrMapSym::names(Array<NetAddr*> &nms) const {
	return getNetAddrs(strNames, nms);
}

const ArraySym *AddrMapSym::addressesSym() const {
	return getArraySym(strAddresses);
}

const ArraySym *AddrMapSym::namesSym() const {
	return getArraySym(strNames);
}

void AddrMapSym::zone(const String &aZone) {
	setString(strZone, aZone);
}

void AddrMapSym::absorbAddressesSym(ArraySym *addresses) {
	return absorbArraySym(strAddresses, addresses);
}

void AddrMapSym::absorbNamesSym(ArraySym *names) {
	return absorbArraySym(strNames, names);
}

AddrMapSym *AddrMapSym::adapt(const int family, const bool clearPorts) const {
	ArraySym *names2 = new ArraySym(NetAddrSym::TheType);
	ArraySym *addresses2 = new ArraySym(NetAddrSym::TheType);
	const int port2 = -1;

	const ArraySym *addresses1 = addressesSym();
	const ArraySym *names1 = namesSym();
	int port1 = -1;
	// for now, we only support N:N maps (e.g., those loaded by dnsZone())
	Assert(addresses1->count() == names1->count());
	for (int i = 0; i < addresses1->count(); ++i) {
		const NetAddrSym &addr1 = (const NetAddrSym&)(addresses1->item(i)->cast(NetAddrSym::TheType));
		if (addr1.val().addrN().family() == family) {
			const NetAddrSym &name1 = (const NetAddrSym&)(names1->item(i)->cast(NetAddrSym::TheType));
			addresses2->add(addr1);
			if (clearPorts && port1 < 0)
				port1 = name1.val().port();
			names2->add(name1);
		}
	}

	// if cloning is acceptable, then clone in hope to preserve most info
	if (port1 == port2 && addresses1->count() == addresses2->count()) {
		delete names2;
		delete addresses2;
		return static_cast<AddrMapSym*>(clone());
	}

	ArraySym *namesWithoutPorts2 = 0;
	if (port1 != port2) {
		namesWithoutPorts2 = PglSemx::SetAddrPort(*names2, port2, names1->loc());
		delete names2;
		names2 = 0;
	} else {
		namesWithoutPorts2 = names2;
		names2 = 0;
	}

	addresses2->loc(addresses1->loc());
	namesWithoutPorts2->loc(names1->loc());

	AddrMapSym *map2 = new AddrMapSym;
	map2->loc(loc());
	map2->zone(zone());
	map2->absorbAddressesSym(addresses2);
	map2->absorbNamesSym(namesWithoutPorts2);
	return map2;
}
