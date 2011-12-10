/*
 * readJPEG.h
 *
 *  Created on: 02.12.2011
 *      Author: id23cat
 */

#ifndef READJPEG_H_
#define READJPEG_H_

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;

static const int ZigZag[8][8]={
		{ 0,  1,  5,  6, 14, 15, 27, 28},
		{ 2,  4,  7, 13, 16, 26, 29, 42},
		{ 3,  8, 12, 17, 25, 30, 41, 43},
		{ 9, 11, 18, 24, 31, 40, 44, 53},
		{10, 19, 23, 32, 39, 45, 52, 54},
		{20, 22, 33, 38, 46, 51, 55, 60},
		{21, 34, 37, 47, 50, 56, 59, 61},
		{35, 36, 48, 49, 57, 58, 62, 63}
};

struct JComment{			// comment
	UINT16 length;
	char *str;
};

#define WORDLEN_1BYTE 0		// 1 byte length
#define WORDLEN_2BYTE 1		// 2 bytes length

#define _Y 1
#define _CB 2
#define _CR 3

struct DQT{					// Quantization table
	UINT16 length;
	UINT8 tElLength;		// one element length in bytes: WORDLEN_1BYTE or WORDLEN_2BYTE
	UINT8 qtID;				// Quantization table ID: _Y, _CB, _CR
	UINT8 qTable8[8][8];
	UINT16 qTable16[8][8];
};


struct COLCOMPONENT{			// color component
	UINT8 cID;					// component ID
	UINT8 horizontalDecimation;
	UINT8 verticalDecimation;
	UINT8 qtID;					// Quantization table ID in DQT: _Y, _CB, _CR
};

struct SOF0{					// Baseline DCT
	UINT16 length;
	UINT8 precision;			// more often =3
	UINT16 imHeight;
	UINT16 imWidth;
	UINT8 componentsCount;
	struct COLCOMPONENT Y;
	struct COLCOMPONENT Cb;
	struct COLCOMPONENT Cr;
};

#define DC 0
#define AC 1

struct DHT{				// Huffman table
	UINT16 length;
	UINT8 type;			//DC or AC
	UINT8 htID;			// Huffman table ID: _Y, _CB, CR
	// --------!!!--------
};

struct COMPONENT{
	UINT8 colID;		// _Y, _CB, _CR
	UINT8 hufDCID;
	UINT8 *hufDC;
	UINT8 hufACID;
	UINT8 *hufAC;
};
struct SOS{
	UINT16 headlen;
	UINT16 compCount;		// more often =3
	struct COMPONENT Y;
	struct COMPONENT Cb;
	struct COMPONENT Cr;
};

struct JPEG{
	struct JComment	comment;
	struct DQT 		qTable;
	struct SOF0		properties;
	struct DHT		hTable;
	struct SOS		scan;
};

int parseJpeg(FILE *jfile);

#endif /* READJPEG_H_ */
