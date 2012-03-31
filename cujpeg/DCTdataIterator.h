/*
 * DCTdataIterator.h
 *
 *  Created on: 12.01.2012
 *      Author: id23cat
 */

#ifndef DCTDATAITERATOR_H_
#define DCTDATAITERATOR_H_

#include "datatypes.h"
#include "Exceptions.h"


//namespace Iterators {
//template <typename T>
//class JustVector {
//protected:
//		T *data;
//		long long dataLength;
//public:
//		JustVector(): data(NULL), dataLength(0){};
//		JustVector(T *d, long long dl): data(d), dataLength(dl* sizeof(T)) {};
//		JustVector(JustVector<T> &it): data(it.data), dataLength(it.dataLength){};
//		~JustVector() {data = NULL; dataLength=0;};
//		virtual T& operator[](long long idx)=0;
//		long long length() {return dataLength;};
//};
//
////template <typename T>
//class DCTdataIterator :public JustVector<INT16>{
//	static int objectCount;
////	static DCTdataIterator *IteratorObjects;
//
//	INT16 *curBlkPtr;
//	int curColor;
//	int curColIdx;
//
//	int colorCount;
//	UINT8 decimation[3];
//
//public:
//	DCTdataIterator(){
//		//	////// object counter
//		objectCount++;
//		this->data = NULL;
//		curBlkPtr = NULL;
//
//		colorCount = 0;
//		decimation = {0};
//		curColor = _Y;
//		curColIdx = 0;
//	};
//	DCTdataIterator(INT16 *d, long long dl, UINT8 Decimation[3]): JustVector<INT16>(d, dl){
//		assert(d);
//		//	////// object counter
//		objectCount++;
//		curBlkPtr = this->data;
//		assert(dl);
//
//		colorCount = 3;
//		for (int i = 0; i < colorCount; i++)
//			decimation[i] = Decimation[i];
//		curColor = _Y;
//		curColIdx = 0;
//	};
//	DCTdataIterator(INT16 *d, long long dl): JustVector<INT16>(d, dl){
//		assert(d);
//		//	////// object counter
//		objectCount++;
//		curBlkPtr = this->data;
//		assert(dl);
//		colorCount = 3;
//		decimation[0] = 4;
//		decimation[1] = 1;
//		decimation[2] = 1;
//		curColor = _Y;
//		curColIdx = 0;
//	};
//	DCTdataIterator(DCTdataIterator &it): JustVector<INT16>(it){
//		assert(it.curBlkPtr);
//		//	////// object counter
//		objectCount++;
//		curBlkPtr = it.curBlkPtr;
//		assert(it.dataLength);
//		curColor = it.curColor;
//		curColIdx = it.curColIdx;
//		colorCount = it.colorCount;
//		for (int i = 0; i < colorCount; i++) {
//			decimation[i] = it.decimation[i];
//		}
//	};
//	virtual ~DCTdataIterator(){
//		objectCount--;
//		curBlkPtr = NULL;
////		dataLength = 0;
//		curColor = 0;
//		curColIdx = 0;
//		colorCount = 0;
//	};
//
//	int color() {
//		return curColor;
//	}
//	;
//
//	bool lastBlock() {
//		return curBlkPtr + BLK_LENGTH >= this->data + this->dataLength;
//	}
//	;
//
//	bool firstBlock() {
//		return curBlkPtr - BLK_LENGTH < this->data;
//	}
//	;
//
//	DCTdataIterator& mvToNextBlock() throw (indexing_fail){ // move current pointer to next block & return current pointer
//			if (curBlkPtr + BLK_LENGTH >= this->data + this->dataLength) {
//				throw indexing_fail(__FILE__, __LINE__,
//						"DCTdataIterator::mvToNextBlock(): out of index");
//			}
//			curBlkPtr += BLK_LENGTH;
//
//			if (curColIdx < decimation[curColor] - 1)
//				curColIdx++;
//			else {
//				if (curColor < _CR) {
//					curColor++;
//					curColIdx = 0;
//				} else {
//					curColor = _Y;
//					curColIdx = 0;
//				}
//			}
//
//			return *this;
//	};
//	DCTdataIterator& NextBlock() throw (indexing_fail){ // return pointer to next block (curBlkPtr does not change)
//		try {
//				return DCTdataIterator(*this).mvToNextBlock();
//			} catch (indexing_fail &exc) {
//				exc << str_info("From DCTdataIterator::NextBlock()");
//				throw exc;
//		}
//	};
//	DCTdataIterator& mvToPrevBlock() throw (indexing_fail){ // move current pointer to previous block & return current pointer
//		if (curBlkPtr - BLK_LENGTH < this->data) {
//				throw indexing_fail(__FILE__, __LINE__,
//						"DCTdataIterator::mvToNextBlock(): out of index");
//			}
//			curBlkPtr -= BLK_LENGTH;
//			if (curColIdx > 0)
//				curColIdx--;
//			else {
//				if (curColor > _Y) {
//					curColor--;
//					curColIdx = decimation[curColor] - 1;
//				} else {
//					curColor = _CR;
//					curColIdx = decimation[curColor] - 1;
//				}
//			}
//
//		return *this;
//	};
//	DCTdataIterator PrevBlock() throw (indexing_fail){ // return pointer to previous block (curBlkPtr does not change)
//		try {
//				return DCTdataIterator(*this).mvToPrevBlock();
//		} catch (indexing_fail &exc) {
//				exc << str_info("From DCTdataIterator::PrevBlock()");
//				throw exc;
//		}
//	};
//	INT16 getPrevDC() throw (indexing_fail){ 		// return DC coefficient from prev. block from curColor
//		try {
//				if (curBlkPtr == this->data && curColIdx == 0) {
//					return 0;
//				} else if (curColor == _Y && curColIdx > 0) {
//					INT16 t = PrevBlock()[0];
//					//		printf("PrevDC = %d\n", t);
//					//		delete tmpIt;
//					//		tmpIt = NULL;
//					return t;
//					//		return PrevBlock()[0];
//				} else if (curColor == _Y && curColIdx == 0) {
//					INT16 t = PrevBlock(). PrevBlock(). PrevBlock()[0];
//
//					//			PrevBlock();
//					//			PrevBlock();
//					//			PrevBlock();
//					//			PrevBlock()[0];
//					return t;
//				} else if (curColor != _Y && curColIdx == 0) {
//					INT16 t = 0;
//					try {
//						//move to previous block group
//						t
//								= PrevBlock().PrevBlock(). PrevBlock().PrevBlock(). PrevBlock(). PrevBlock()[0];
//					} catch (indexing_fail) {
//						//				printf("\nFirst group\n");
//						return 0;
//					}
//					return t;
//				} else
//					return 0;
//			} catch (indexing_fail &exc) {
//				exc << str_info("From DCTdataIterator::getPrevDC()");
//				throw exc;
//			}
//	};
//	INT16* getCurBlock() {
//		return curBlkPtr;
//	};
//
//	INT16& operator[](long long idx) throw (indexing_fail){
//		if (idx < 0 || idx >= 64) {
//				char str[256];
//				sprintf(str, "DCTdataIterator::operator[]( %lld ): out of index", idx);
//				throw indexing_fail(__FILE__, __LINE__, str);
//			}
//			if (!curBlkPtr||(curBlkPtr + sizeof(INT16) * idx >= this->data + this->dataLength)) {
//				char str[256];
//				sprintf(str, "DCTdataIterator::operator[]( %lld ): out of index:\n", idx);
//				sprintf(str,
//						"%s\t%d(tcurBlkPtr+sizeof(<INT16>)*idx) >= %d(data+dataLength)",
//						str, (int) (curBlkPtr + sizeof(INT16) * idx),
//						(int) (this->data + this->dataLength));
//				throw indexing_fail(__FILE__, __LINE__, str);
//			}
//			return curBlkPtr[idx];
//	};
//	INT16& LineView(int y, int x){ 				// Line indexing by 2d matrix view of DCT block
//		return curBlkPtr[y * 8 + x];
//	};
//	INT16& ZigZagView(int y, int x){ 			// ZigZag indexing by 2d matrix view of DCT block
//		return curBlkPtr[Natural_order2D[y][x]];
//	};
//	INT16& LineView(int x){ 					// Line indexing by vector view of DCT block
//		return curBlkPtr[x];
//	};
//	INT16& ZigZagView(int x){ 					// ZigZag indexing by vector view of DCT block
//		return curBlkPtr[Natural_order[x]];
//	};
//
//	void PrintData() throw (indexing_fail){
//		printf(
//					"DCTdataIterator: data=[%p..%p], curBlkPtr=%p, Length=%lld, curColIdx=%d\n",
//					this->data, this->data + this->dataLength, curBlkPtr, this->dataLength, curColIdx);
//
//			DCTdataIterator it = *this;
//			int j = 0;
//			try {
//				it.mvToPrevBlock();
//			} catch (indexing_fail e) {
//				printf("Prev block does not exist\n");
//				j++;
//			}
//			for (; j < 3; j++) {
//				for (int i = 0; i < 64; i++)
//					{printf("%d ", it[i]); fflush(stdout);}
//				printf("\n");
//
//				if (j != 2)
//					try {
//						it.mvToNextBlock();
//					} catch (indexing_fail &e) {
//						printf("Next block does not exist\n");
//						return;
//					}
//			}
//	};
//};

//template<typename T>
//int DCTdataIterator<T>::objectCount;


//template <class T>
//class KZdataIterator: public DCTdataIterator{
//public:
//	KZdataIterator(){};
//	KZdataIterator(KZdataIterator &it) :
//		DCTdataIterator(it) {};
//	KZdataIterator(INT16 *d, long long dl, UINT8 Decimation[3]) :
//		DCTdataIterator(d, dl, Decimation) {};
////	KZdataIterator(INT16 *d, long long dl, UINT8 Decimation[3]) :
////		DCTdataIterator<INT16>(d, dl, Decimation) {};
//	KZdataIterator(INT16 *d, long long dl) :
//			DCTdataIterator(d, dl) {};
//	virtual ~KZdataIterator(){};
//	INT16& operator[](int idx) throw (indexing_fail){
//		if (idx < 0) {
//				char str[256];
//				sprintf(str, "KZdataIterator::operator[]( %d ): out of index", idx);
//				throw indexing_fail(__FILE__, __LINE__, str);
//			}
//			if (idx >= 8) {
//				idx = 7;
//			}
//			try {
//
//				return DCTdataIterator::operator[](KochZhaoZZ_order[idx]);
//
//			} catch (indexing_fail &exc) {
//				exc << str_info("From KZdataIterator::operator[]");
//				throw exc;
//			}
//	}
//	KZdataIterator& operator&(KZdataIterator it) throw (indexing_fail){
//		*this = it;
//		return *this;
//	}
//};


class BITSETiterator {
	size_t dataLength;
	UINT8 *data;
	size_t byteIdx;
	UINT8 bitIdx;
	bool bitdebug;
public:
	BITSETiterator() :
		dataLength(0), data(NULL), byteIdx(0), bitIdx(0), bitdebug(false) {
	}
	;
	BITSETiterator(UINT8 *d, long dl) :
		dataLength(dl), data(d), byteIdx(0), bitIdx(0), bitdebug(false) {
	}
	;
	~BITSETiterator() {
		data = NULL;
	}
	;
	BITSETiterator Begin() {
		byteIdx = 0;
		bitIdx = 0;
		return *this;
	}
	;
	BITSETiterator NextBit() {
		if (bitIdx < 7)
			bitIdx++;
		else if (byteIdx < dataLength - 1) {
			byteIdx++;
			bitIdx = 0;
		} else
			throw 1;
		return *this;
	}
	;
	UINT8 GetBit() {
		if(bitdebug)
				printf(" %d ", data[byteIdx] >> (7 - bitIdx) & 1);		// bit-debug
		fflush(stdout);
		return data[byteIdx] >> (7 - bitIdx) & 1;
	}
	;
	void PrintData() {
		printf("\n");
		for (size_t i = byteIdx; i < dataLength; i++)
			printf(" %x", (int) data[i]);
		printf("\n");
	}
	;
	void PrintData(int len) {
		printf("\n\t[");
		for (size_t i = byteIdx; i < byteIdx + len; i++)
			printf(" %X", (int) data[i]);
		printf("]\n");
	}
	;
	void BitDebug(bool flg=false){
		bitdebug = flg;
	}
};


//template <typename T>
//class DCTdataIterator :public JustVector<T>{
//	static int objectCount;
////	static DCTdataIterator *IteratorObjects;
//
//	T *curBlkPtr;
//	int curColor;
//	int curColIdx;
//
//	int colorCount;
//	UINT8 decimation[3];
//
//public:
//	DCTdataIterator(){
//		//	////// object counter
//		objectCount++;
//		this->data = NULL;
//		curBlkPtr = NULL;
//
//		colorCount = 0;
//		decimation = {0};
//		curColor = _Y;
//		curColIdx = 0;
//	};
//	DCTdataIterator(T *d, long long dl, UINT8 Decimation[3]): JustVector<T>(d, dl){
//		assert(d);
//		//	////// object counter
//		objectCount++;
////		this->data = d;
//		curBlkPtr = this->data;
//		assert(dl);
////		dataLength = dl * sizeof(T);
//
//		colorCount = 3;
//		for (int i = 0; i < colorCount; i++)
//			decimation[i] = Decimation[i];
//		curColor = _Y;
//		curColIdx = 0;
//	};
//	DCTdataIterator(T *d, long long dl): JustVector<T>(d, dl){
//		assert(d);
//		//	////// object counter
//		objectCount++;
////		this->data = d;
//		curBlkPtr = this->data;
//		assert(dl);
////		dataLength = dl * sizeof(T);
//		colorCount = 3;
//		decimation[0] = 4;
//		decimation[1] = 1;
//		decimation[2] = 1;
//		curColor = _Y;
//		curColIdx = 0;
//	};
//	DCTdataIterator(DCTdataIterator &it): JustVector<T>(it){
//		assert(it.curBlkPtr);
//		//	////// object counter
//		objectCount++;
////		this->data = it.data;
//		curBlkPtr = it.curBlkPtr;
//		assert(it.dataLength);
////		dataLength = it.dataLength;
//		curColor = it.curColor;
//		curColIdx = it.curColIdx;
//		colorCount = it.colorCount;
//		for (int i = 0; i < colorCount; i++) {
//			decimation[i] = it.decimation[i];
//		}
//	};
//	virtual ~DCTdataIterator(){
//		objectCount--;
//		curBlkPtr = NULL;
////		dataLength = 0;
//		curColor = 0;
//		curColIdx = 0;
//		colorCount = 0;
//	};
//
//	int color() {
//		return curColor;
//	}
//	;
//
//	bool lastBlock() {
//		return curBlkPtr + BLK_LENGTH >= this->data + this->dataLength;
//	}
//	;
//
//	bool firstBlock() {
//		return curBlkPtr - BLK_LENGTH < this->data;
//	}
//	;
//
//	DCTdataIterator& mvToNextBlock() throw (indexing_fail){ // move current pointer to next block & return current pointer
//			if (curBlkPtr + BLK_LENGTH >= this->data + this->dataLength) {
//				throw indexing_fail(__FILE__, __LINE__,
//						"DCTdataIterator::mvToNextBlock(): out of index");
//			}
//			curBlkPtr += BLK_LENGTH;
//
//			if (curColIdx < decimation[curColor] - 1)
//				curColIdx++;
//			else {
//				if (curColor < _CR) {
//					curColor++;
//					curColIdx = 0;
//				} else {
//					curColor = _Y;
//					curColIdx = 0;
//				}
//			}
//
//			return *this;
//	};
//	DCTdataIterator& NextBlock() throw (indexing_fail){ // return pointer to next block (curBlkPtr does not change)
//		try {
//				return DCTdataIterator(*this).mvToNextBlock();
//			} catch (indexing_fail &exc) {
//				exc << str_info("From DCTdataIterator::NextBlock()");
//				throw exc;
//		}
//	};
//	DCTdataIterator& mvToPrevBlock() throw (indexing_fail){ // move current pointer to previous block & return current pointer
//		if (curBlkPtr - BLK_LENGTH < this->data) {
//				throw indexing_fail(__FILE__, __LINE__,
//						"DCTdataIterator::mvToNextBlock(): out of index");
//			}
//			curBlkPtr -= BLK_LENGTH;
//			if (curColIdx > 0)
//				curColIdx--;
//			else {
//				if (curColor > _Y) {
//					curColor--;
//					curColIdx = decimation[curColor] - 1;
//				} else {
//					curColor = _CR;
//					curColIdx = decimation[curColor] - 1;
//				}
//			}
//
//		return *this;
//	};
//	DCTdataIterator PrevBlock() throw (indexing_fail){ // return pointer to previous block (curBlkPtr does not change)
//		try {
//				return DCTdataIterator(*this).mvToPrevBlock();
//		} catch (indexing_fail &exc) {
//				exc << str_info("From DCTdataIterator::PrevBlock()");
//				throw exc;
//		}
//	};
//	T getPrevDC() throw (indexing_fail){ 		// return DC coefficient from prev. block from curColor
//		try {
//				if (curBlkPtr == this->data && curColIdx == 0) {
//					return 0;
//				} else if (curColor == _Y && curColIdx > 0) {
//					T t = PrevBlock()[0];
//					//		printf("PrevDC = %d\n", t);
//					//		delete tmpIt;
//					//		tmpIt = NULL;
//					return t;
//					//		return PrevBlock()[0];
//				} else if (curColor == _Y && curColIdx == 0) {
//					T t = PrevBlock(). PrevBlock(). PrevBlock()[0];
//
//					//			PrevBlock();
//					//			PrevBlock();
//					//			PrevBlock();
//					//			PrevBlock()[0];
//					return t;
//				} else if (curColor != _Y && curColIdx == 0) {
//					T t = 0;
//					try {
//						//move to previous block group
//						t
//								= PrevBlock().PrevBlock(). PrevBlock().PrevBlock(). PrevBlock(). PrevBlock()[0];
//					} catch (indexing_fail) {
//						//				printf("\nFirst group\n");
//						return 0;
//					}
//					return t;
//				} else
//					return 0;
//			} catch (indexing_fail &exc) {
//				exc << str_info("From DCTdataIterator::getPrevDC()");
//				throw exc;
//			}
//	};
//	T* getCurBlock() {
//		return curBlkPtr;
//	};
//
//	T& operator[](long long idx) throw (indexing_fail){
//		if (idx < 0 || idx >= 64) {
//				char str[256];
//				sprintf(str, "DCTdataIterator::operator[]( %lld ): out of index", idx);
//				throw indexing_fail(__FILE__, __LINE__, str);
//			}
//			if (!curBlkPtr||(curBlkPtr + sizeof(T) * idx >= this->data + this->dataLength)) {
//				char str[256];
//				sprintf(str, "DCTdataIterator::operator[]( %lld ): out of index:\n", idx);
//				sprintf(str,
//						"%s\t%d(tcurBlkPtr+sizeof(<T>)*idx) >= %d(data+dataLength)",
//						str, (int) (curBlkPtr + sizeof(T) * idx),
//						(int) (this->data + this->dataLength));
//				throw indexing_fail(__FILE__, __LINE__, str);
//			}
//			return curBlkPtr[idx];
//	};
//	T& LineView(int y, int x){ 				// Line indexing by 2d matrix view of DCT block
//		return curBlkPtr[y * 8 + x];
//	};
//	T& ZigZagView(int y, int x){ 			// ZigZag indexing by 2d matrix view of DCT block
//		return curBlkPtr[Natural_order2D[y][x]];
//	};
//	T& LineView(int x){ 					// Line indexing by vector view of DCT block
//		return curBlkPtr[x];
//	};
//	T& ZigZagView(int x){ 					// ZigZag indexing by vector view of DCT block
//		return curBlkPtr[Natural_order[x]];
//	};
//
//	void PrintData() throw (indexing_fail){
//		printf(
//					"DCTdataIterator: data=[%p..%p], curBlkPtr=%p, Length=%lld, curColIdx=%d\n",
//					this->data, this->data + this->datadataLength, curBlkPtr, this->datadataLength, curColIdx);
//
//			DCTdataIterator it = *this;
//			int j = 0;
//			try {
//				it.mvToPrevBlock();
//			} catch (indexing_fail e) {
//				printf("Prev block does not exist\n");
//				j++;
//			}
//			for (; j < 3; j++) {
//				for (int i = 0; i < 64; i++)
//					{printf("%d ", it[i]); fflush(stdout);}
//				printf("\n");
//
//				if (j != 2)
//					try {
//						it.mvToNextBlock();
//					} catch (indexing_fail &e) {
//						printf("Next block does not exist\n");
//						return;
//					}
//			}
//	};
//};
//
//template<typename T>
//int DCTdataIterator<T>::objectCount;
//
//
//template <class T>
//class KZdataIterator: public DCTdataIterator <T>{
//public:
//	KZdataIterator(){};
//	KZdataIterator(KZdataIterator &it) :
//		DCTdataIterator<T>(it) {};
//	KZdataIterator(T *d, long long dl, UINT8 Decimation[3]) :
//		DCTdataIterator<T>(d, dl, Decimation) {};
//	KZdataIterator(INT16 *d, long long dl, UINT8 Decimation[3]) :
//		DCTdataIterator<INT16>(d, dl, Decimation) {};
//	KZdataIterator(T *d, long long dl) :
//			DCTdataIterator<T>(d, dl) {};
//	virtual ~KZdataIterator(){};
//	T& operator[](int idx) throw (indexing_fail){
//		if (idx < 0) {
//				char str[256];
//				sprintf(str, "KZdataIterator::operator[]( %d ): out of index", idx);
//				throw indexing_fail(__FILE__, __LINE__, str);
//			}
//			if (idx >= 8) {
//				idx = 7;
//			}
//			try {
//
//				return DCTdataIterator<T>::operator[](KochZhaoZZ_order[idx]);
//
//			} catch (indexing_fail &exc) {
//				exc << str_info("From KZdataIterator::operator[]");
//				throw exc;
//			}
//	}
//	KZdataIterator& operator&(KZdataIterator it) throw (indexing_fail){
//		*this = it;
//		return *this;
//	}
//};
//
//
//class BITSETiterator {
//	size_t dataLength;
//	UINT8 *data;
//	size_t byteIdx;
//	UINT8 bitIdx;
//public:
//	BITSETiterator() :
//		dataLength(0), data(NULL), byteIdx(0), bitIdx(0) {
//	}
//	;
//	BITSETiterator(UINT8 *d, long dl) :
//		dataLength(dl), data(d), byteIdx(0), bitIdx(0) {
//	}
//	;
//	~BITSETiterator() {
//		data = NULL;
//	}
//	;
//	BITSETiterator Begin() {
//		byteIdx = 0;
//		bitIdx = 0;
//		return *this;
//	}
//	;
//	BITSETiterator NextBit() {
//		if (bitIdx < 7)
//			bitIdx++;
//		else if (byteIdx < dataLength - 1) {
//			byteIdx++;
//			bitIdx = 0;
//		} else
//			throw 1;
//		return *this;
//	}
//	;
//	UINT8 GetBit() {
//		//		printf(" %d ", data[byteIdx] >> (7 - bitIdx) & 1);
//		fflush(stdout);
//		return data[byteIdx] >> (7 - bitIdx) & 1;
//	}
//	;
//	void PrintData() {
//		printf("\n");
//		for (size_t i = byteIdx; i < dataLength; i++)
//			printf(" %x", (int) data[i]);
//		printf("\n");
//	}
//	;
//	void PrintData(int len) {
//		printf("\n\t[");
//		for (size_t i = byteIdx; i < byteIdx + len; i++)
//			printf(" %X", (int) data[i]);
//		printf("]\n");
//	}
//	;
//};

#endif /* DCTDATAITERATOR_H_ */
