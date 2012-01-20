/*
 * DCTdataIterator.cpp
 *
 *  Created on: 12.01.2012
 *      Author: id23cat
 */

#include "DCTdataIterator.h"
#include <assert.h>

DCTdataIterator::DCTdataIterator() {
	// TODO Auto-generated constructor stub
	data = NULL;
	curPtr = NULL;
	dataLength = 0;

	colorCount = 0;
	width = 0;
	height = 0;
	hDecimation = {0};
	vDecimation = {0};
	curColor = _Y_;
	curColIdx = 0;
}

DCTdataIterator::DCTdataIterator(INT16 *d, int dl, SOF0 config) {
	assert(d);
	data = d;
	curPtr = data;
	assert(dl);
	dataLength = dl;

	colorCount = config.componentsCount;
	width = config.imWidth;
	height = config.imHeight;

	for (int i = 0; i < colorCount; i++) {
		hDecimation[i] = config.component[i].horizontalDecimation;
		vDecimation[i] = config.component[i].verticalDecimation;
	}
	curColor = _Y_;
	curColIdx = 0;
}

DCTdataIterator::DCTdataIterator(DCTdataIterator &it) {
	assert(it.data);
	data = it.data;
	curPtr = it.curPtr;
	assert(it.dataLength);
	dataLength = it.dataLength;

	curColor = it.curColor;
	curColIdx = it.curColIdx;

	colorCount = it.colorCount;
	width = it.width;
	height = it.height;

	for (int i = 0; i < colorCount; i++) {
		hDecimation[i] = it.hDecimation[i];
		vDecimation[i] = it.vDecimation[i];
	}
}

DCTdataIterator::~DCTdataIterator() {
	// TODO Auto-generated destructor stub
}

DCTdataIterator& DCTdataIterator::mvToNextBlock() throw (int) {
	if (curPtr + BLK_LENGTH_BYTE >= data + dataLength) {
		//		throw 1;
		fprintf(stderr,
				"DCTdataIterator::mvToNextBlock(): out of index at %s :%d",
				__FILE__, __LINE__);
		throw 1;
	}
	curPtr += BLK_LENGTH_BYTE;

	if (curColIdx < hDecimation[curColor] * vDecimation[curColor] - 1)
		curColIdx++;
	else {
		if (curColor < _CR) {
			curColor++;
			curColIdx = 0;
		} else {
			curColor = _Y;
			curColIdx = 0;
		}
	}

	return *this;
}

DCTdataIterator& DCTdataIterator::NextBlock() throw (int) {
	return DCTdataIterator(*this).mvToNextBlock();
}

DCTdataIterator& DCTdataIterator::mvToPrevBlock() throw (int) {
	if (curPtr - BLK_LENGTH_BYTE < data) {
		//		throw 1;
		fprintf(stderr,
				"DCTdataIterator::mvToPrevBlock(): out of index at %s :%d",
				__FILE__, __LINE__);
		throw 1;
	}
	curPtr -= BLK_LENGTH_BYTE;

	if (curColIdx > 0)
		curColIdx--;
	else {
		if (curColor > _Y) {
			curColor--;
			curColIdx = hDecimation[curColor] * vDecimation[curColor] - 1;
		} else {
			curColor = _CR;
			curColIdx = hDecimation[curColor] * vDecimation[curColor] - 1;
		}
	}

	return *this;
}

DCTdataIterator& DCTdataIterator::PrevBlock() throw (int) {
	return DCTdataIterator(*this).mvToPrevBlock();
}

INT16 DCTdataIterator::getPrevDC() {
	if (curColIdx > 0) {
		INT16 t = PrevBlock()[0];
		//		printf("PrevDC = %d\n", t);
//		delete tmpIt;
//		tmpIt = NULL;
		return t;
		//		return PrevBlock()[0];
	} else
		return 0;
}

INT16& DCTdataIterator::operator[](int idx) {
	if (idx < 0 || idx >= 64 || (curPtr + sizeof(INT16) * idx >= data
			+ dataLength)) {
		fprintf(stderr,
				"DCTdataIterator::operator[%d]: out of index at %s :%d", idx,
				__FILE__, __LINE__);
		exit(1);
	}
	//if ()
	return curPtr[idx];
}

