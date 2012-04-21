/*
 * KZanalizer.cpp
 *
 *  Created on: 30.01.2012
 *      Author: id23cat
 */

#include "KZanalizer.h"
#include <vector>

KZanalizer::KZanalizer(JPEG::DCTdataIterator begin, JPEG::DCTdataIterator end, UINT8 comp) {
	colorComponent = comp;
	// calculate elements count
	INT16* ptr1 = begin.getCurBlock();
	INT16* ptr2 = end.getCurBlock();

	size_t count = (ptr2 - ptr1);
	blockCount = count/64;

	dctLen = blockCount * 8;
	if(colorComponent != _ALL){
		UINT8* decim = begin.getDecimation();
		dctLen = dctLen * decim[colorComponent]/ (decim[_Y] + decim[_CB] + decim[_CR]);
	}
	SAFE_MALLOC_INT16(dctPtr, dctLen);


	KZdataIterator kzit(dctPtr, dctLen, begin.getDecimation());
	JPEG::DCTdataIterator it = begin;

	printf("KZ constructor\n");
	while(it < end){
		if(colorComponent == _ALL || colorComponent == it.color()){
			for(int i=0; i<8; i++)
				kzit[i] = it[KochZhaoZZ_order[i]];
//			it.PrintBlock();
//			printf("\n");
//			kzit.PrintBlock();
//			printf("\n");
			kzit.mvToNextBlock();
		}
		it.mvToNextBlock();
	}
}

KZanalizer::KZanalizer(INT16 *data, size_t datalen){
	size_t blkcount = datalen / 64;
	dctLen = blkcount * 8;
	SAFE_MALLOC_INT16(dctPtr, dctLen);

	KZdataIterator kzit(dctPtr, dctLen, defaultDecimation);
	JPEG::DCTdataIterator it(data, datalen, defaultDecimation);

	while (it < it.end()) {
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

bool KZanalizer::Analize(){
	KZdataIterator it(dctPtr, dctLen);
	KZdataIterator end = it;
	end.end();			// move to end
	float* stdArray;
	SAFE_MALLOC(stdArray, blockCount,float);

	size_t i=0;
	while(it < end){
		stdArray[i++] = it.SqrDeviation();
		printf("std = %f\n",stdArray[i-1]);
		it.mvToNextBlock();
	}
	std::vector<HIST> hist;
	build_hist(hist, stdArray, 0.05f, 0.0001f, 3.f);

	SAFE_FREE(stdArray);
}

float KZanalizer::KZdataIterator::Mean(){
	long int sum = 0;
	for(size_t i=0; i<DCTdataIterator::blkSize; i++)
		sum += this->operator[](i);
	return (float)sum/(float)DCTdataIterator::blkSize;
}

float KZanalizer::KZdataIterator::SqrDeviation(){
	float dev = 0;
	float mean = Mean();
	float d = 0;
	for(size_t i=0; i<DCTdataIterator::blkSize; i++){
		d = this->operator[](i) - mean;
		dev += (float)d*d;
	}
	return sqrt(dev/(float)(DCTdataIterator::blkSize-1));
}

void KZanalizer::build_hist(std::vector<HIST> &hist, float *data, VALUETYPE begin, VALUETYPE dist, VALUETYPE end){

	VALUETYPE k = 1/dist;
	UINT16 x0 = begin * k;
	UINT16 z = 0;

	HIST h;

	for(VALUETYPE j = begin; /*i<hist->length && */j<=end; j+=dist){
		h.value = j;
		h.count = 0;
		hist.push_back(h);
//		printf("%.5f ", j);
	}
	printf("\n");
	printf("%5f\n", dist);

	for(size_t i=0; i<blockCount; i++){
		if(data[i] < begin || data[i] > end) continue;
		z = data[i] * k - x0;
		hist[z].count++;
		printf("%f v[%d]=%f, count=%d\n",hist[z].value, z, data[i], hist[z].count);
	}
}
