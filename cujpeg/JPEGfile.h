/*
 * JPEGfile.h
 *
 *  Created on: 05.12.2011
 *      Author: id23cat
 */

#ifndef JPEGFILE_H_
#define JPEGFILE_H_

#include "StreamReader.h"
#include <boost/dynamic_bitset.hpp>
#include "huftree.h"

using namespace std;

#define LOW(byte) (byte & 0x0F)
#define HIGH(byte) (byte >> 4)

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;

static const int ZigZag[8][8] = {
		{ 0,  1,  5,  6, 14, 15, 27, 28 },
		{ 2,  4,  7, 13, 16, 26, 29, 42 },
		{ 3,  8, 12, 17, 25, 30, 41, 43 },
		{ 9, 11, 18, 24, 31, 40, 44, 53 },
		{10, 19, 23, 32, 39, 45, 52, 54 },
		{20, 22, 33, 38, 46, 51, 55, 60 },
		{21, 34, 37, 47, 50, 56, 59, 61 },
		{35, 36, 48, 49, 57, 58, 62, 63 }};


template<typename T>
int fillZigZag(T M[8][8], T *v) {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			M[i][j] = v[ZigZag[i][j]];
	return 0;
}


struct JComment { // comment
	UINT16 length;
	char *str;
	void PrintData(){printf("Comment (%d bytes): \"%s\"\n", length, str);};
};

#define WORDLEN_1BYTE 0		// 1 byte length
#define WORDLEN_2BYTE 1		// 2 bytes length
#define _Y_ 0
#define _CBCR_ 1
#define _Y 1
#define _CB 2
#define _CR 3

struct DQT { // Quantization table
	UINT16 length;
	UINT8 tElLength; // one element length in bytes: WORDLEN_1BYTE or WORDLEN_2BYTE
	UINT8 qtID; // Quantization table ID: _Y, _CB, _CR
	UINT8 qTable8[64];
	UINT16 qTable16[64];
	void PrintData(){
		if(length){
			printf("DQT(%d bytes): qtID=%d,\n",length, qtID);
			printf("\tqTable(typesize=%d): ", tElLength);
			for(int i=0; i<64; i++)
				printf("%x ", qTable8[i]);
			printf("\n");
		}
	};
};

#define _DC 0
#define _AC 1

struct DHT { // Huffman table
	UINT16 length;
	UINT8 cls; //_DC or _AC
	UINT8 htID; // Huffman table ID: _Y, _CB, _CR
	// --------!!!--------
	UINT8 codeCount[16];	// 1 to 16
	int mcodeslength;
	UINT8 *code;
	// --------Tree--------
//	tree<NODE> tr;
	TREE tree;

	void CalcTree();
 	void PrintData(){
		if(length){
			printf("DHT (%d bytes): DC/AC=%d, htID=%d\n", length, cls, htID);
			printf("\tcodeCount: ");
			for(int i=0;i<16;i++)
				printf("%x ", codeCount[i]);
			printf("\n");
			printf("\tCode (%d): ", mcodeslength);
			for(int i=0;i<mcodeslength;i++)
				printf("%x ", code[i]);
			printf("\n");
		}
	}
};

//typedef UINT16[64] DCTblock;

struct COLCOMPONENT { // color component
	UINT8 cID; // component ID
	UINT8 horizontalDecimation;
	UINT8 verticalDecimation;
	UINT8 qtID; // Quantization table ID in DQT: _Y, _CB, _CR

	// from SOS
	UINT8 hufDCID;		// index of Huffman config for DC coefficient
	struct DHT *hufDC;		// pointer to DC-Huffman-tree struct
	UINT8 hufACID;		// index of Huffman config for AC coefficient
	struct DHT *hufAC;		// pointer to AC-Huffman-tree struct

//	DCTblock **blockM;
//	DCTblock *blockV;
	void PrintData(){
		printf("ColorComponent ID=%d: hPrecision=%d, vPrecision=%d, qtID=%d\n", cID, horizontalDecimation,
				verticalDecimation, qtID);
		printf("\tCOMPONENT: cID=%d, hufDCID=%d, hufACID=%d\n", cID, hufDCID, hufACID);
				printf("\t\tHUFtableDC: ");
//				hufDC->PrintData();
				printf("\t\tHUFtableAC: ");
//				hufAC->PrintData();
	}
};

struct SOF0 { // Baseline DCT
	UINT16 length;
	UINT8 precision; // more often =3
	UINT16 imHeight;
	UINT16 imWidth;
	UINT8 componentsCount;
	UINT8 *srcDataPtr;
	size_t dataLength;
//	UINT8 compCount;
//	struct COLCOMPONENT Y;
//	struct COLCOMPONENT Cb;
//	struct COLCOMPONENT Cr;
	struct COLCOMPONENT component[3];
	void PrintData(){
		if(length){
			printf("SOF0 (%d bytes): precision=%d, height=%d, width=%d, componentsCount=%d\n",
					length,
					precision,
					imHeight, imWidth,
					componentsCount);
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

class BITSETiterator{
	long dataLength;
	UINT8 *data;
	UINT8 byteIdx;
	UINT8 bitIdx;
public:
	BITSETiterator(): dataLength(0), data(NULL), byteIdx(0), bitIdx(0){};
	BITSETiterator(UINT8 *d, long dl): dataLength(dl), data(d), byteIdx(0), bitIdx(0){};
	~BITSETiterator(){data=NULL;};
	BITSETiterator Begin(){byteIdx = 0; bitIdx = 0; return *this;};
	BITSETiterator NextBit(){
		if(bitIdx<7)
			bitIdx++;
		else if(byteIdx < dataLength-1){
			byteIdx++;
			bitIdx = 0;
		}else throw 1;
		return *this;
	};
	UINT8 GetBit(){ return data[byteIdx]>>(7-bitIdx)&1;};
};

class JPEG {
//	friend class BITSETiterator;

	struct JComment comment;
//	struct DQT qTableY;
//	struct DQT qTableCbCr;
	struct DQT qTable[2];
	struct SOF0 data;
	struct DHT hTable[2][2];	// [tableID][DC/AC]
//	map<UINT8, struct DHT, Compare> mm;
//	struct SOS scan;
//	long dataLength;
//	UINT8 *dataPtr;
	short DCTs[64];

	StreamReader jfile;
private:
	UINT16 readFlag();
	int readComment();
	int readDQT();
	int readSOF0();
	int readDHT();
	int readSOS();
	void deleteJPEG();
	int parseJpeg();

public:
	JPEG(char *jfname);
	virtual ~JPEG();
	void GetDCTs();
};

#endif /* JPEGFILE_H_ */
