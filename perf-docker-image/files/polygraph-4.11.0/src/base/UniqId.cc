
/* Web Polygraph       http://www.web-polygraph.org/
 * Copyright 2003-2011 The Measurement Factory
 * Licensed under the Apache License, Version 2.0 */

#include "base/polygraph.h"

#include <limits.h>
#include <stdlib.h>
#include "xstd/h/os_std.h"
#include "xstd/h/process.h" /* for _getpid() on W2K */
#include "xstd/h/iostream.h"
#include "xstd/h/sstream.h"
#include "xstd/h/iomanip.h"
#include "xstd/h/string.h"

#include "base/ILog.h"
#include "base/OLog.h"
#include "base/BStream.h"
#include "base/UniqId.h"
#include "xstd/FNVHash.h"
#include "xstd/gadgets.h"

static UniqId TheSessionId;
static int TheSpaceId = 0;


/* UniqId::UuId */

UniqId::UuId::UuId() {
	memset(this, 0, sizeof(*this));
}

void UniqId::UuId::finalize(const uint8_t version) {
	data3 = (data3 & 0xfff) | uint16_t(version) << 12;
	data4[0] = (data4[0] & 0x3F) | 0x80; // variant 1 (of version 4)
}

ostream &UniqId::UuId::print(ostream &os) const {
	const ios_fmtflags flags = os.flags();
	const char fill = os.fill('0');
	os << hex
		<< setw(8) << data1 << '-'
		<< setw(4) << data2 << '-'
		<< setw(4) << data3 << '-'
		<< setw(2) << int(data4[0]) << setw(2) << int(data4[1]) << '-';

	for (int i = 2; i < 8; ++i)
		os << setw(2) << int(data4[i]);

	os.fill(fill);
	os.flags(flags);
	return os;
}


/* UniqId */

void UniqId::Space(int id) {
	// we must be called before any ids are generated (weak check)
	Assert(TheSessionId.theCnt <= 0); 
	Assert(id >= 0); 
	TheSpaceId = id;
}

UniqId::UniqId(): theSecs(0), theMix(0), theCnt(0) {
}

UniqId::UniqId(int aSecs, int aMix, int aCnt): theSecs(aSecs), theMix(aMix), theCnt(aCnt) {
	Assert(aCnt <= 0 || (aSecs > 0 && aMix > 0));
}

UniqId &UniqId::create() {
	if (TheSessionId.theCnt <= 0)
		RefreshSessionId();
	*this = TheSessionId;
	TheSessionId.theCnt += 2;
	return *this;
}

int UniqId::hash() const {
	unsigned int hash = FNV32Hash(theSecs);
	hash = FNV32Hash(theMix, hash);
	hash = FNV32Hash(theCnt, hash);
	return Abs(static_cast<int>(hash));
}

UniqId::UuId UniqId::uuid4() const {
	UuId uuid;
	if (*this) {
		uuid.data1 = theSecs;
		uuid.data2 = uint32_t(theMix) >> 16;
		uuid.data4[2] = uint32_t(theMix) >> 8;
		uuid.data4[3] = uint32_t(theMix);
		uuid.data4[4] = uint32_t(theCnt) >> 24;
		uuid.data4[5] = uint32_t(theCnt) >> 16;
		uuid.data4[6] = uint32_t(theCnt) >> 8;
		uuid.data4[7] = uint32_t(theCnt);
		uuid.finalize(4); // v4
	}
	return uuid;
}

ostream &UniqId::print(ostream &os) const {
	const ios_fmtflags flags = os.flags();
	const char fill = os.fill('0');
	os << hex
		<< setw(8) << theSecs << '.' 
		<< setw(8) << theMix << ':' 
		<< setw(8) << theCnt;
	os.fill(fill);
	os.flags(flags);
	return os;
}

OLog &UniqId::store(OLog &ol) const {
	return ol << theSecs << theMix << theCnt;
}

ILog &UniqId::load(ILog &il) {
	return il >> theSecs >> theMix >> theCnt;
}

OBStream &UniqId::store(OBStream &os) const {
	return os << theSecs << theMix << theCnt;
}

IBStream &UniqId::load(IBStream &is) {
	return is >> theSecs >> theMix >> theCnt;
}

String UniqId::str() const {
	char buf[64];
	ofixedstream os(buf, sizeof(buf));
	print(os) << ends;
	return buf;
}

UniqId UniqId::FromStr(const Area &area) {
	int buf[3] = { 0, 0, 0 };
	memcpy(&buf, area.data(), Min(area.size(), (int)sizeof(buf)));

	// make secs and mix positive
	buf[0] = buf[0] <= 0 ? buf[0] + INT_MAX : buf[0];
	buf[1] = buf[1] <= 0 ? buf[1] + INT_MAX : buf[1];
	buf[2] = Min(buf[2] <= 0 ? buf[2] + INT_MAX : buf[2], INT_MAX - 2);

	// make theCnt even (and still positive)
	buf[2] /= 2;
	buf[2] *= 2;
	buf[2] += 2;

	return UniqId(buf[0], buf[1], buf[2]);
}

// it would be better to use hostid in a mix, but gethostid() is not portable
// so we use tm.usec instead
void UniqId::RefreshSessionId() {
	if (!TheSpaceId) {
		// use random factors to reset "session id"
		static unsigned pid = (unsigned) getpid();
		Time tm(Time::Now());

		TheSessionId = UniqId(
			0x7fffffff & (int)(tm.sec() - 912345678),
			0x7fffffff & (int)((tm.usec() << 16) | (pid & 0xffff)),
			2);
	} else {
		// stay within the specified space
		TheSessionId = UniqId(
			TheSpaceId,
			TheSessionId.theMix+1,
			2);
	}
}

bool UniqId::parse(const char *&buf, const char *) {
	int a = 0, b = 0, c = 0;
	const char *p = 0;
	if (!(isInt(buf, a, &p, 16) && *p == '.' &&
		isInt(p+1, b, &p, 16) && *p == ':' &&
		isInt(p+1, c, &buf, 16)))
		return false;

	if (c <= 0 || (a > 0 && b > 0)) {
		theSecs = a;
		theMix = b;
		theCnt = c;
		return *this != 0;
	}

	return false;

}
