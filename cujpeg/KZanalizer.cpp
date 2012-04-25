/*
 * KZanalizer.cpp
 *
 *  Created on: 30.01.2012
 *      Author: id23cat
 */

#include "KZanalizer.h"
#include <vector>
#include <algorithm>

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

//	printf("KZ constructor\n");
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

bool KZanalizer::Analize(int Pthreshold){
	KZdataIterator it(dctPtr, dctLen);
	KZdataIterator end = it;
	end.end();			// move to end
	float* stdArray;
	SAFE_MALLOC(stdArray, blockCount,float);

	size_t i=0;
	while(it < end){
		stdArray[i++] = it.SqrDeviation();
//		printf("std = %f\n",stdArray[i-1]);
		it.mvToNextBlock();
	}
	std::vector<HIST> hist;
	VALUETYPE N = (VALUETYPE)build_hist(hist, stdArray, 0.05f, 0.0001f, 3.f);
	Compare compobj;
	std::vector<HIST>::iterator maxit = std::max_element(hist.begin(), hist.end(), compobj);

	printf("maxValue = %f, maxCount = %d\n", (*maxit).value, (*maxit).count);

	// compare with sigma=0.354 value
	if((*maxit).value > SIGMA){
		probability = 100;
		return true;		// this is stogo!
	}

	VALUETYPE Nsmax = (VALUETYPE)(*maxit).count;
	VALUETYPE P = Nsmax/N *100;
	printf("P = %f\%\n", P);

	float t = P - (int)P;
	float k;
	if(P < 23){
		probability = 27.333f;
	}else if(P > 23 && P <= 24){
		k = (57 - 50) * t;
		probability = 50 + k;
		printf("23-24 = %f\n",probability);
	}else if(P > 24 && P <= 25){
		k = (65 - 57) * t;
		probability = 57 + k;
		printf("24-25 = %f\n",probability);
	}else if(P > 25 && P <= 26){
		k = (71 - 65) * t;
		probability = 65 + k;
		printf("25-26 = %f\n",probability);
	}else if(P > 26 && P <= 27){
		k = (77 - 71) * t;
		probability = 71 + k;
		printf("26-27 = %f\n",probability);
	}else if(P > 27 && P <= 28){
		k = (83 - 77) * t;
		probability = 77 + k;
		printf("27-28 = %f\n", probability);
	} else if (P > 28 && P <= 29) {
		k = (85 - 83) * t;
		probability = 83 + k;
		printf("28-29 = %f\n", probability);
	} else if (P > 29 && P <= 30) {
		k = (87 - 83) * t;
		probability = 83 + k;
		printf("29-30 = %f\n", probability);
	} else if (P > 30 && P <= 31) {
		k = (91 - 87) * t;
		probability = 87 + k;
		printf("30-31 = %f\n", probability);
	} else if (P > 31 && P <= 32) {
		k = (100 - 91) * t;
		probability = 91 + k;
		printf("31-32 = %f\n", probability);
	} else if (P > 32) {
		probability = 100;
	}

	if(probability < Pthreshold)
		return false; 		// this is not stego
	else
		return true;		// this is stego

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

size_t KZanalizer::build_hist(std::vector<HIST> &hist, float *data, VALUETYPE begin, VALUETYPE dist, VALUETYPE end){

	size_t totcount = 0;
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
//	printf("\n");
//	printf("%5f\n", dist);

	for(size_t i=0; i<blockCount; i++){
		if(data[i] < begin || data[i] > end) continue;
		z = data[i] * k - x0;
		hist[z].count++;
		totcount++;
//		printf("%f v[%d]=%f, count=%d\n",hist[z].value, z, data[i], hist[z].count);
	}
	return totcount;
}
