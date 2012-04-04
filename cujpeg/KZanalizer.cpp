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

	KZdataIterator kzit(dctPtr, dctLen, begin.getDecimation());
	JPEG::DCTdataIterator it = begin;

	while(it < end){
		for(int i=0; i<8; i++)
			kzit[i] = it[KochZhaoZZ_order[i]];
		kzit.mvToNextBlock();
		it.mvToNextBlock();
	}
}

KZanalizer::KZanalizer(INT16 *d, size_t dl){
	size_t blkcount = dl / 64;
	dctLen = blkcount * 8;
	SAFE_MALLOC_INT16(dctPtr, dctLen);
}

KZanalizer::~KZanalizer() {
	free(dctPtr);
}


//INT16& KZanalizer::KZdataIterator::operator[](int idx) throw (indexing_fail) {
//	if (idx < 0) {
//		char str[256];
//		sprintf(str, "KZdataIterator::operator[]( %d ): out of index", idx);
//		throw indexing_fail(__FILE__, __LINE__, str);
//	}
//	if (idx >= 8) {
//		idx = 7;
//	}
//	try {
//
//		return JPEG::DCTdataIterator::operator[](KochZhaoZZ_order[idx]);
//
//	} catch (indexing_fail &exc) {
//		exc << str_info("From KZdataIterator::operator[]");
//		throw exc;
//	}
//}

//KZanalizer::KZdataIterator& KZanalizer::KZdataIterator::operator&(KZanalizer::KZdataIterator it) throw (indexing_fail) {
//	*this = it;
//	return *this;
//}
