/*
 * DCTdataIterator.h
 *
 *  Created on: 12.01.2012
 *      Author: id23cat
 */

#ifndef DCTDATAITERATOR_H_
#define DCTDATAITERATOR_H_
#include "JPEGfile.h"
#include "Exceptions.h"


//#ifndef INT16
//typedef signed short INT16;
//#endif

/*
 *
 */
class DCTdataIterator {
	INT16 *data;
	long long dataLength;
	INT16 *curBlkPtr;
	int curColor;
	int curColIdx;

	int colorCount;
	int width;
	int height;
	int hDecimation[3];
	int vDecimation[3];

//	DCTdataIterator *tmpIt;
public:
	DCTdataIterator();
	DCTdataIterator(INT16 *d, long long dl, SOF0 config);
	DCTdataIterator(DCTdataIterator &it);
	virtual ~DCTdataIterator();

	int color(){return curColor;};

	bool lastBlock(){return curBlkPtr + BLK_LENGTH >= data + dataLength;};
	bool firstBlock(){return curBlkPtr - BLK_LENGTH < data;};
	DCTdataIterator& mvToNextBlock()throw (indexing_fail);		// move current pointer to next block & return current pointer
	DCTdataIterator& NextBlock()throw (indexing_fail);			// return pointer to next block (curBlkPtr does not change)
	DCTdataIterator& mvToPrevBlock()throw (indexing_fail);		// move current pointer to previous block & return current pointer
	DCTdataIterator PrevBlock()throw (indexing_fail);			// return pointer to previous block (curBlkPtr does not change)
	INT16 getPrevDC()throw (indexing_fail);	// return DC coefficient from prev. block from curColor
	INT16* getCurBlock(){return curBlkPtr;};

	INT16& operator[](int idx) throw (indexing_fail);
	INT16& LineView(int y, int x);				// Line indexing by 2d matrix view of DCT block
	INT16& ZigZagView(int y, int x);			// ZigZag indexing by 2d matrix view of DCT block
	INT16& LineView(int x);						// Line indexing by vector view of DCT block
	INT16& ZigZagView(int x);					// ZigZag indexing by vector view of DCT block

	void PrintData()throw (indexing_fail);
};


#endif /* DCTDATAITERATOR_H_ */
