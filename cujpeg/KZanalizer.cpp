/*
 * KZanalizer.cpp
 *
 *  Created on: 30.01.2012
 *      Author: id23cat
 */

#include "KZanalizer.h"

#define DEFAULT_DECIMATION  {4,1,1}

KZanalizer::KZanalizer(JPEG::DCTdataIterator begin, JPEG::DCTdataIterator end) {
	// calculate elements count
	INT16* ptr1 = begin.getCurBlock();
	INT16* ptr2 = end.getCurBlock();

	size_t count = (ptr2 - ptr1);
	size_t blkcount = count/64;

	dctLen = blkcount * 8;
	SAFE_MALLOC_INT16(dctPtr, dctLen);

	KZdataIterator kzit(dctPtr, dctLen, begin.getDecimation());
	JPEG::DCTdataIterator it = begin;

	while(it < end){
		for(int i=0; i<8; i++)
			kzit[i] = it[KochZhaoZZ_order[i]];
		it.PrintBlock();
		printf("\n");
		kzit.PrintBlock();
		printf("\n");
		kzit.mvToNextBlock();
		it.mvToNextBlock();
	}
}

KZanalizer::KZanalizer(INT16 *data, size_t ddatalen){
	size_t blkcount = datalen / 64;
	dctLen = blkcount * 8;
	SAFE_MALLOC_INT16(dctPtr, dctLen);

	UINT8 decim[3] = DEFAULT_DECIMATION;
	KZdataIterator kzit(dctPtr, dctLen, begin.getDecimation());
	JPEG::DCTdataIterator it(data, datalen, decim);

	while (it < end) {
		for (int i = 0; i < 8; i++)
			kzit[i] = it[KochZhaoZZ_order[i]];
		it.PrintBlock();
		printf("\n");
		kzit.PrintBlock();
		printf("\n");
		kzit.mvToNextBlock();
		it.mvToNextBlock();
	}
}

KZanalizer::~KZanalizer() {
	SAFE_FREE(dctPtr);
}

