/*
 * DCTdataIterator.h
 *
 *  Created on: 12.01.2012
 *      Author: id23cat
 */

#ifndef DCTDATAITERATOR_H_
#define DCTDATAITERATOR_H_
#include "JPEGfile.h"


//#ifndef INT16
//typedef signed short INT16;
//#endif

/*
 *
 */
class DCTdataIterator {
	INT16 *data;
	int dataLength;
	INT16 *curPtr;
	int curColor;
	int curColIdx;

	int colorCount;
	int width;
	int height;
	int hDecimation[3];
	int vDecimation[3];

	DCTdataIterator *tmpIt;
public:
	DCTdataIterator();
	DCTdataIterator(INT16 *d, int dl, SOF0 config);
	DCTdataIterator(DCTdataIterator &it);
	virtual ~DCTdataIterator();

	int color(){return curColor;};

	bool lastBlock(){return curPtr + BLK_LENGTH_BYTE >= data + dataLength;};
	bool firstBlock(){return curPtr - BLK_LENGTH_BYTE < data;};
	DCTdataIterator& mvToNextBlock()throw(int);		// move current pointer to next block & return current pointer
	DCTdataIterator& NextBlock()throw(int);			// return pointer to next block (curPtr does not change)
	DCTdataIterator& mvToPrevBlock()throw(int);		// move current pointer to previous block & return current pointer
	DCTdataIterator& PrevBlock()throw(int);			// return pointer to previous block (curPtr does not change)
	INT16 getPrevDC();	// return DC coefficient from prev. block from curColor

	INT16& operator[](int idx);
};


#endif /* DCTDATAITERATOR_H_ */
