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

//struct HISTOGRAM_ITERATOR;
//typedef HISTOGRAM_ITERATOR HISTIT;

//struct HISTOGRAM;
//
//struct HISTOGRAM_ITERATOR{
//	HISTOGRAM hist;
//		size_t cind;
//		size_t valind;
//public:
//	HISTOGRAM_ITERATOR(HIST &hst): hist(hst), cind(0), valind(0){};
//
//	HISTOGRAM_ITERATOR first(){cind = 0; valind = 0; return *this;};
//	HISTOGRAM_ITERATOR last(){cind = hist.length-1; valind = hist.length-1; return *this;};
//
//};

typedef struct HISTOGRAM{
	INT16 count;
	VALUETYPE value;
} HIST;


class KZanalizer {
	INT16 *dctPtr;				// coefficients pointer
	size_t dctLen;				// elements count
	size_t blockCount;
	UINT8 colorComponent;			// color component (_Y, _CB, _CR, _ALL)
private:
	void build_hist(std::vector<HIST> &hist, float *data, VALUETYPE begin=0.05f, VALUETYPE dist=0.0001f, VALUETYPE end=3.f);
public:
	KZanalizer(): dctPtr(NULL), dctLen(0){};
	KZanalizer(JPEG::DCTdataIterator begin, JPEG::DCTdataIterator end, UINT8 component=_ALL);	// take iterators
	KZanalizer(INT16 *data, size_t datalen);				// take pointer to DCT sequence & count
	virtual ~KZanalizer();

	bool Analize();

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

//		DCTdataIterator& mvToNextBlock() throw (indexing_fail){ return DCTdataIterator::mvToNextBlock();}
		//		KZdataIterator begin(){return this.}
		//		KZdataIterator end(){}

//		INT16& operator[](int idx) throw (indexing_fail); // move to next item in block
//		INT16& operator++() throw (indexing_fail); // move to next block
//		KZdataIterator& mvToNextBlock() throw (indexing_fail); // move current pointer to next block & return current pointer
//		KZdataIterator& mvToPrevBlock() throw (indexing_fail); // move current pointer to previous block & return current pointer

//		KZdataIterator& operator&(KZdataIterator it) throw (indexing_fail);
	};
};

#endif /* KZANALIZER_H_ */
