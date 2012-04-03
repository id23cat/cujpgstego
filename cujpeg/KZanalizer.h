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


class KZanalizer {
	INT16 *dctPtr;
	size_t dctLen;
public:
	KZanalizer(): dctPtr(NULL), dctLen(0){};
	KZanalizer(JPEG::DCTdataIterator &begin, JPEG::DCTdataIterator &end);	// take iterators
	KZanalizer(INT16 *d, size_t dl);				// take pointer to DCT sequence & count
	virtual ~KZanalizer();

	class KZdataIterator: protected DCTdataIterator {
	public:
		KZdataIterator() {
		};
		KZdataIterator(INT16* data, size_t dLength, UINT8 decim[3]) :
			DCTdataIterator(data, dLength, decim) {
		};

		KZdataIterator(const KZdataIterator &it) :
			DCTdataIterator(it) {
		}
		;
		virtual ~KZdataIterator() {
		}
		;

		//		KZdataIterator begin(){return this.}
		//		KZdataIterator end(){}

		INT16& operator[](int idx) throw (indexing_fail); // move to next item in block
//		INT16& operator++() throw (indexing_fail); // move to next block
		KZdataIterator& mvToNextBlock() throw (indexing_fail); // move current pointer to next block & return current pointer
		KZdataIterator& mvToPrevBlock() throw (indexing_fail); // move current pointer to previous block & return current pointer

		KZdataIterator& operator&(KZdataIterator it) throw (indexing_fail);
	};
};

#endif /* KZANALIZER_H_ */
