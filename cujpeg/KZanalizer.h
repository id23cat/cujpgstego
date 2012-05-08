/*
 * KZanalizer.h
 *
 *  Created on: 30.01.2012
 *      Author: id23cat
 */

#ifndef KZANALIZER_H_
#define KZANALIZER_H_
#include "datatypes.h"
#include "JPEGfile.h"

#define KZ_BLK_LENGTH 8
#define DEFAULT_DECIMATION  {4,1,1}

#ifndef DEFAULTDECIMATION
static UINT8 defaultDecimation[3] = DEFAULT_DECIMATION;
#endif

#define VALUETYPE float
#define SIGMA 0.354f

typedef struct HISTOGRAM{
	UINT32 count;
	VALUETYPE value;
} HIST;

struct Compare {
	bool operator()(HIST h1, HIST h2){
		return h1.count < h2.count;
	}
};

class KZanalizer {
protected:

	INT16 *dctPtr;				// coefficients pointer
	size_t dctLen;				// elements count
	size_t blockCount;
	UINT8 colorComponent;		// color component (_Y, _CB, _CR, _ALL)
	VALUETYPE probability;			// probability of stego
private:
	size_t build_hist(std::vector<HIST> &hist, float *data, VALUETYPE begin=0.05f, VALUETYPE dist=0.0001f, VALUETYPE end=3.f);
public:
	KZanalizer(): dctPtr(NULL), dctLen(0){};
	KZanalizer(JPEG::DCTdataIterator begin, JPEG::DCTdataIterator end, UINT8 component=_ALL);	// take iterators
	KZanalizer(INT16 *data, size_t datalen);				// take pointer to DCT sequence & count
	virtual ~KZanalizer();

	// Analize:
	// true -- contain stego
	// false -- clean image
	bool Analize(int Pthreshold = 70);		// Pthreshold -- threshold for P-value

	// call after Analize -- return result probability
	VALUETYPE GetProbability() {return probability;};

	// return pointer to dctPtr -- vector of Koch-Zhao 8/block coefficient;
	INT16 *Get8DCTsPointer() {return dctPtr;};

	class KZdataIterator: public JPEG::DCTdataIterator {
	public:
		KZdataIterator() {
			blkSize = KZ_BLK_LENGTH;
		};

		KZdataIterator(INT16* data, size_t dLength, UINT8 decim[3]=defaultDecimation) :
			JPEG::DCTdataIterator(data, dLength, decim) {
			blkSize = KZ_BLK_LENGTH;
		};

		KZdataIterator(const KZdataIterator &it) :
			DCTdataIterator(it) {};

		virtual ~KZdataIterator() {};

		float Mean();
		float SqrDeviation();
	};
};

class KZanalizerCUDA: public KZanalizer {
	INT16 *dDCTptr;			// pointer in device memory
	VALUETYPE *dMean;
	VALUETYPE *dStd;
	VALUETYPE *dSum;
//	HIST *gHist;
public:
	KZanalizerCUDA(JPEG::DCTdataIterator begin, JPEG::DCTdataIterator end, UINT8 component=_ALL):
	KZanalizer(begin, end, component){}; // take iterators
//	KZanalizerCUDA(INT16 *data, size_t datalen); // take pointer to DCT sequence & count
	virtual ~KZanalizerCUDA();
	// Analize:
	// true -- contain stego
	// false -- clean image
	bool Analize(int Pthreshold = 70);		// Pthreshold -- threshold for P-value
private:
	int InitMem();

};

#endif /* KZANALIZER_H_ */
