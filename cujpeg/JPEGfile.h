/*
 * JPEGfile.h
 *
 *  Created on: 05.12.2011
 *      Author: id23cat
 */

#ifndef JPEGFILE_H_
#define JPEGFILE_H_


#include "DCTdataIterator.h"
#include "StreamReader.h"
//#include <boost/dynamic_bitset.hpp>
#include "huftree.h"
#include "datatypes.h"

using namespace std;
//using namespace Iterators;

//class KZdataIterator;



template<typename T>
int fillZigZag(T M[8][8], T *v) {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			M[i][j] = v[ZigZag_order2D[i][j]];
	return 0;
}

//struct JFIF {
//	UINT16 length;
//	UINT8 id[5];
//	UINT16 version;
//	UINT8 units;
//	UINT16 xDensity;
//	UINT16 yDensity;
//	UINT8 xThumbnail;
//	UINT8 yThumbnail;
//	UINT8 n[3];
//	void PrintData(){
//		printf("JFIF (%d bytes):\n", length);
//		printf(" %s\n", id);
//		printf("Version %d, units %d, Xdensity=%d, Ydensity=%d\n", version, units, xDensity, yDensity);
//		printf("Xthumbnail=%d, Ythumbnail=%d\n", xThumbnail, yThumbnail);
////		printf("sizeof(JFIF)=%d", sizeof(JFIF));
//	}
//};

struct JComment { // comment
	UINT16 length;
	char *str;
	void PrintData() {
		printf("Comment (%d bytes): \"%s\"\n", length, str);
	}
	;
};



struct DQT { // Quantization table
	UINT16 length;
	UINT8 tElLength; // one element length in bytes: WORDLEN_1BYTE or WORDLEN_2BYTE
	UINT8 qtID; // Quantization table ID: _Y, _CB, _CR
	UINT8 qTable8[64];
	UINT16 qTable16[64];
	void PrintData() {
		if (length) {
			printf("DQT(%d bytes): qtID=%d,\n", length, qtID);
			printf("\tqTable(typesize = %d bytes): ", tElLength+1);
			for (int i = 0; i < 64; i++)
				printf("%x ", qTable8[i]);
			printf("\n");
		}
	}
	;
};

struct DHT { // Huffman table
	UINT16 length;
	UINT8 cls; //_DC or _AC
	UINT8 htID; // Huffman table ID: _Y, _CB, _CR
	// --------!!!--------
	UINT8 codeCount[16]; // 1 to 16
	int mcodeslength;
	UINT8 *code;
	// --------Tree--------
	//	tree<NODE> tr;
	TREE tree;

	void CalcTree();
	void PrintData() {
		if (length) {
			printf("DHT (%d bytes): DC/AC=%d, htID=%d\n", length, cls, htID);
			printf("\tcodeCount: ");
			for (int i = 0; i < 16; i++)
				printf("%x ", codeCount[i]);
			printf("\n");
			printf("\tCode (%d): ", mcodeslength);
			for (int i = 0; i < mcodeslength; i++)
				printf("%x ", code[i]);
			printf("\n");
			tree.PrintData();
		}
	}
};

//typedef UINT16[64] DCTblock;

struct COLCOMPONENT { // color component
	COLCOMPONENT() {
		cID = 0;
		horizontalDecimation = 0;
		verticalDecimation = 0;
		qtID = 0;
		hufDCID = 0;
		hufDC = NULL;
		hufACID = 0;
		hufAC = NULL;
	}
	UINT8 cID; // component ID
	UINT8 horizontalDecimation;
	UINT8 verticalDecimation;
	UINT8 qtID; // Quantization table ID in DQT: _Y, _CB, _CR

	// from SOS
	UINT8 hufDCID; // index of Huffman config for DC coefficient
	struct DHT *hufDC; // pointer to DC-Huffman-tree struct
	UINT8 hufACID; // index of Huffman config for AC coefficient
	struct DHT *hufAC; // pointer to AC-Huffman-tree struct

	//	DCTblock **blockM;
	//	DCTblock *blockV;
	void PrintData() {
		printf("ColorComponent ID=%d: hPrecision=%d, vPrecision=%d, qtID=%d\n",
				cID, horizontalDecimation, verticalDecimation, qtID);
		printf("\tCOMPONENT: cID=%d, hufDCID=%d, hufACID=%d\n", cID, hufDCID,
				hufACID);
		printf("\t\tHUFtableDC: ");
		//				hufDC->PrintData();
		printf("\t\tHUFtableAC: ");
		//				hufAC->PrintData();
		printf("\n");
	}
};

struct SOF0 { // Baseline DCT
	SOF0() {
		length = 0;
		precision = 0;
		imHeight = 0;
		imWidth = 0;
		componentsCount = 0;
		srcDataPtr = NULL;
		srcDataLength = 0;
		maxVDecimation = 0;
		maxHDecimation = 0;
	}
	UINT16 length;
	UINT8 precision; // more often =3
	UINT16 imHeight; // in pixels
	UINT16 imWidth; // *********
	UINT8 componentsCount;
	UINT8 *srcDataPtr;
	size_t srcDataLength;
	//	UINT8 compCount;
	//	struct COLCOMPONENT Y;
	//	struct COLCOMPONENT Cb;
	//	struct COLCOMPONENT Cr;
	struct COLCOMPONENT component[3];
	UINT8 maxVDecimation;
	UINT8 maxHDecimation;
	UINT8 decimation[3];
	int memmv1bLeft(UINT8 *start, int len);
	void PrintSrcData(){
		for(size_t i=0; i<srcDataLength; i++)
			printf("%02X ", srcDataPtr[i]);
		printf("\n");
	}
	void PrintData() {
		if (length) {
			printf(
					"SOF0 (%d bytes): precision=%d, height=%d, width=%d, componentsCount=%d\n",
					length, precision, imHeight, imWidth, componentsCount);
			//			Y.PrintData();
			//			Cb.PrintData();
			//			Cr.PrintData();
			component[0].PrintData();
			component[1].PrintData();
			component[2].PrintData();
		}
	}
};

//struct COMPONENT {
//	UINT8 colID; // _Y, _CB, _CR
//	UINT8 hufDCID;
////	UINT8 *hufDC;
//	struct DHT *hufDC;
//	UINT8 hufACID;
////	UINT8 *hufAC;
//	struct DHT *hufAC;
////	DCTblock **blockM;
////	DCTblock *blockV;
//
//	void PrintData(){
//		printf("\tCOMPONENT: colID=%d, hufDCID=%d, hufACID=%d\n", colID, hufDCID, hufACID);
//		printf("\t\tHUFtableDC: ");
//		hufDC->PrintData();
//		printf("\t\tHUFtableAC: ");
//		hufAC->PrintData();
//	}
//};
//
//struct SOS {
//	UINT16 headlen;
//	UINT8 compCount; // more often =3
////	struct COMPONENT Y;
////	struct COMPONENT Cb;
////	struct COMPONENT Cr;
//	struct COMPONENT component[3];
//	void PrintData(){
//		printf("SOS (headlen=%d bytes): compCount=%d\n", headlen, compCount);
//		component[0].PrintData();
//		component[1].PrintData();
//		component[2].PrintData();
////		Y.PrintData();
////		Cb.PrintData();
////		Cr.PrintData();
//	}
//};

//struct Compare{
//	bool operator()(const UINT8 u1, const UINT8 u2){
//		return u1==u2;
//	}
//};

//class DCTdataIterator;

//struct DCT{
//	INT16 *data;
//	size_t DCTlength;
//	DCT(DCT& src){ data = src.data; DCT
//};

//struct KZDCT: public DCT{
//	INT16 *Ydata;
//	size_t Ylength;
//	INT16 *Cbdata;
//	size_t Cblength;
//	INT16 *Crdata;
//	size_t Crlength;
//};

class JPEG {
	StreamReader jfile;		// stream descriptor

	struct JComment comment;
	struct DQT qTable[2];
	struct SOF0 data;
	struct DHT hTable[2][2]; // [tableID][DC/AC]

private:
//	DCT DCTsdata;
	INT16 *DCTdata;
	size_t DCTdataLength;		// count of all DCTs

private:
	UINT16 readFlag();		// read 2 bytes
	int readComment();		// read comment section from a stream
	int readJFIF();			// read JFIF section
	int readDQT();			// read DQT section
	int readSOF0();			// read SOF0 section
	int readDHT();			// ..
	int readSOS();			// ..
	//void deleteJPEG();
	int parseJpeg();		// read jpeg sections and put data to respective structures
public:
	class DCTdataIterator /*:public JustVector<INT16>*/{
		//JPEG *jpeg;
		INT16* beginAddr;
		INT16* endAddr;
		size_t dataLength; 	// in DCT coefficients
		UINT8* decimation;	// decimation table
	protected:
		UINT8 blkSize;

		//		static int objectCount;
		//	static DCTdataIterator *IteratorObjects;

		INT16 *curBlkPtr;
		int curColor;
		int curColIdx;

		int colorCount;
		//		UINT8 decimation[3];
	private:
		void selfCheckNULL(std::string file, int line) throw (NULLptr_fail);	// self checking for NULL pointers of critical parameters

	public:
		DCTdataIterator();
		DCTdataIterator(JPEG *j)throw (NULLptr_fail);
		DCTdataIterator(INT16* data, size_t dLength, UINT8 decim[3])throw (NULLptr_fail);
		DCTdataIterator(const DCTdataIterator &it)throw (NULLptr_fail);
		virtual ~DCTdataIterator();

		int color();
		bool lastBlock()throw (NULLptr_fail);
		bool firstBlock()throw (NULLptr_fail);

		DCTdataIterator& begin()throw (NULLptr_fail);
		DCTdataIterator& end()throw (NULLptr_fail);

		DCTdataIterator& mvToNextBlock() throw (indexing_fail); // move current pointer to next block & return current pointer
		DCTdataIterator& NextBlock() throw (indexing_fail); // return pointer to next block (curBlkPtr does not change)
		DCTdataIterator& mvToPrevBlock() throw (indexing_fail); // move current pointer to previous block & return current pointer
		DCTdataIterator PrevBlock() throw (indexing_fail); // return pointer to previous block (curBlkPtr does not change)
		INT16 getPrevDC() throw (indexing_fail); // return DC coefficient from prev. block from curColor
		INT16* getCurBlock();
		UINT8* getDecimation(){return decimation;}

		INT16& operator[](long long idx) throw (indexing_fail);
		bool operator>(const DCTdataIterator &it) const;
		bool operator<(const DCTdataIterator &it) const;
		bool operator>=(const DCTdataIterator &it) const;
		bool operator<=(const DCTdataIterator &it) const;
		bool operator==(const DCTdataIterator &it) const;

		INT16& LineView(int y, int x); // Line indexing by 2d matrix view of DCT block
		INT16& ZigZagView(int y, int x); // ZigZag indexing by 2d matrix view of DCT block
		INT16& LineView(int x); // Line indexing by vector view of DCT block
		INT16& ZigZagView(int x); // ZigZag indexing by vector view of DCT block

		void PrintData() throw (indexing_fail);
		void PrintBlock() throw (indexing_fail);
	};

//	class KZdataIterator: public DCTdataIterator {
//	public:
//		KZdataIterator(){};
//		KZdataIterator(JPEG *j) :
//			DCTdataIterator(j) {};
//
//		KZdataIterator(const KZdataIterator &it) :
//			DCTdataIterator(it) {};
//		virtual ~KZdataIterator() {};
//
////		KZdataIterator begin(){return this.}
////		KZdataIterator end(){}
//
//		INT16& operator[](int idx) throw (indexing_fail);		// move to next item in block
//		INT16& operator++()	throw(indexing_fail);				// move to next block
//
//		KZdataIterator& operator&(KZdataIterator it) throw (indexing_fail);
//	};

public:
	JPEG(char *jfname);
	virtual ~JPEG();

	void GetDCTs()throw(memory_fail);				// decode DCT coefficients to DCTdata
	bool cmpWith(char *fname)throw(memory_fail);	// compare DCTs with existing log file
	DCTdataIterator begin(){
		return DCTdataIterator(this);
	};

	DCTdataIterator end(){
		return DCTdataIterator(this).end();
	};
//	KZdataIterator<INT16> getKZIterator();			// create iterator for a DCT structures
//	KZdataIterator begin(){
//		return KZdataIterator(this);
//	};
//	KZdataIterator end(){
//		return static_cast<KZdataIterator>(KZdataIterator(this).end());
//	};
	void PrintData(int count=0);		// print DCT data

};
//#endif
#endif /* JPEGFILE_H_ */
