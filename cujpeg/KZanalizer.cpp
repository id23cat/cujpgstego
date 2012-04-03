/*
 * KZanalizer.cpp
 *
 *  Created on: 30.01.2012
 *      Author: id23cat
 */

#include "KZanalizer.h"


KZanalizer::KZanalizer(JPEG::DCTdataIterator &begin, JPEG::DCTdataIterator &end) {
	// calculate elements count
	INT16* ptr1 = begin.getCurBlock();
	INT16* ptr2 = end.getCurBlock();

	size_t count = (ptr2 - ptr1)/sizeof(INT16);
	size_t blkcount = count/64;

	dctLen = blkcount * 8;
	SAFE_MALLOC_INT16(dctPtr, dctLen);

}

KZanalizer::KZanalizer(INT16 *d, size_t dl){
	size_t blkcount = dl / 64;
	dctLen = blkcount * 8;
	SAFE_MALLOC_INT16(dctPtr, dctLen);
}

KZanalizer::~KZanalizer() {
}
