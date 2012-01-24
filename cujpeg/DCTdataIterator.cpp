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
	curBlkPtr = NULL;
	dataLength = 0;

	colorCount = 0;
	width = 0;
	height = 0;
	hDecimation = {0};
	vDecimation = {0};
	curColor = _Y;
	curColIdx = 0;
}

DCTdataIterator::DCTdataIterator(INT16 *d, long long dl, SOF0 config) {
	assert(d);
	data = d;
	curBlkPtr = data;
	assert(dl);
	dataLength = dl;

	colorCount = config.componentsCount;
	width = config.imWidth;
	height = config.imHeight;

	for (int i = 0; i < colorCount; i++) {
		hDecimation[i] = config.component[i].horizontalDecimation;
		vDecimation[i] = config.component[i].verticalDecimation;
	}
	curColor = _Y;
	curColIdx = 0;
}

DCTdataIterator::DCTdataIterator(DCTdataIterator &it) {
	assert(it.data);
	data = it.data;
	curBlkPtr = it.curBlkPtr;
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
	data = NULL;
	curBlkPtr = NULL;
	dataLength = 0;

	curColor = 0;
	curColIdx = 0;

	colorCount = 0;
	width = 0;
	height = 0;
}

DCTdataIterator& DCTdataIterator::mvToNextBlock() throw (indexing_fail) {
	if (curBlkPtr + BLK_LENGTH >= data + dataLength) {
		//		throw 1;
		//		fprintf(stderr,
		//				"DCTdataIterator::mvToNextBlock(): out of index at %s :%d",
		//				__FILE__, __LINE__);
		throw indexing_fail(__FILE__, __LINE__,
				"DCTdataIterator::mvToNextBlock(): out of index");
	}
	curBlkPtr += BLK_LENGTH;

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

DCTdataIterator& DCTdataIterator::NextBlock() throw (indexing_fail) {
	try {
		return DCTdataIterator(*this).mvToNextBlock();
	} catch (indexing_fail &exc) {
		exc << str_info("From DCTdataIterator::NextBlock()");
		throw exc;
	}
}

DCTdataIterator& DCTdataIterator::mvToPrevBlock() throw (indexing_fail) {
	if (curBlkPtr - BLK_LENGTH < data) {
		//		throw 1;
		//		fprintf(stderr,
		//				"DCTdataIterator::mvToPrevBlock(): out of index at %s :%d",
		//				__FILE__, __LINE__);
		throw indexing_fail(__FILE__, __LINE__,
				"DCTdataIterator::mvToNextBlock(): out of index");
	}
	curBlkPtr -= BLK_LENGTH;

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

DCTdataIterator DCTdataIterator::PrevBlock() throw (indexing_fail) {
	try {
		return DCTdataIterator(*this).mvToPrevBlock();
	} catch (indexing_fail &exc) {
		exc << str_info("From DCTdataIterator::PrevBlock()");
		throw exc;
	}
}

INT16 DCTdataIterator::getPrevDC() throw (indexing_fail) {
	try {
		if (curBlkPtr == data && curColIdx == 0) {
			return 0;
		} else if (curColor == _Y && curColIdx > 0) {
			INT16 t = PrevBlock()[0];
			//		printf("PrevDC = %d\n", t);
			//		delete tmpIt;
			//		tmpIt = NULL;
			return t;
			//		return PrevBlock()[0];
		} else if (curColor == _Y && curColIdx == 0) {
			INT16 t = PrevBlock(). PrevBlock(). PrevBlock()[0];

			//			PrevBlock();
			//			PrevBlock();
			//			PrevBlock();
			//			PrevBlock()[0];
			return t;
		} else
			return 0;
	} catch (indexing_fail &exc) {
		exc << str_info("From DCTdataIterator::getPrevDC()");
		throw exc;
	}
}

INT16& DCTdataIterator::operator[](int idx) throw (indexing_fail) {
	if (idx < 0 || idx >= 64) {
		char str[256];
		sprintf(str, "DCTdataIterator::operator[]( %d ): out of index", idx);
		throw indexing_fail(__FILE__, __LINE__, str);
	}
	if (curBlkPtr + sizeof(INT16) * idx >= data + dataLength) {
		char str[256];
		sprintf(str, "DCTdataIterator::operator[]( %d ): out of index:\n", idx);
		sprintf(str,
				"%s\t%d(tcurBlkPtr+sizeof(INT16)*idx) >= %d(data+dataLength)",
				str, (int)(curBlkPtr + sizeof(INT16) * idx), (int)(data + dataLength));
		throw indexing_fail(__FILE__, __LINE__, str);
	}
	//		if (idx < 0 || idx >= 64|| (curBlkPtr + /*sizeof(INT16) **/ idx >= data
	//			+ dataLength)) {
	//		fprintf(stderr,
	//				"DCTdataIterator::operator[%d]: out of index at %s :%d", idx,
	//				__FILE__, __LINE__);
	//		exit(1);
	//		throw indexing_fail(__FILE__, __LINE__,
	//				"DCTdataIterator::operator[]: out of index");
	//	}
	//if ()
	return curBlkPtr[idx];
}

INT16& DCTdataIterator::LineView(int y, int x) {
	return curBlkPtr[y * 8 + x];
}

INT16& DCTdataIterator::ZigZagView(int y, int x) {
	return curBlkPtr[ZigZag[y][x]];
}

INT16& DCTdataIterator::LineView(int x) {
	return curBlkPtr[x];
}

INT16& DCTdataIterator::ZigZagView(int x) {
	return curBlkPtr[ZigZag[(int) x / 8][(int) x % 8]];
}

void DCTdataIterator::PrintData() throw (indexing_fail) {
	printf(
			"DCTdataIterator: data=[%p..%p], curBlkPtr=%p, Length=%lld, curColIdx=%d\n",
			data, data + dataLength, curBlkPtr, dataLength, curColIdx);

	DCTdataIterator it = *this;
	int j = 0;
	try {
		it.mvToPrevBlock();
	} catch (indexing_fail e) {
		printf("Prev block does not exist\n");
		j++;
	}
	for (; j < 3; j++) {
		for (int i = 0; i < 64; i++)
			printf("%d ", it[i]);
		printf("\n");

		if (j != 2)
			try {
				it.mvToNextBlock();
			} catch (indexing_fail &e) {
				printf("Next block does not exist\n");
				return;
			}
	}

}
