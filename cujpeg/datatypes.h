/*
 * datatypes.h
 *
 *  Created on: 31.01.2012
 *      Author: id23cat
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#define LOW(byte) (byte & 0x0F)
#define HIGH(byte) (byte >> 4)

typedef unsigned char UINT8;
typedef signed char INT8;
typedef unsigned short UINT16;
typedef signed short INT16;
typedef unsigned int UINT32;

#define BLK_LENGTH 64
#define BLK_LENGTH_BYTE 64*sizeof(INT16)

#define _DC 0
#define _AC 1

#define WORDLEN_1BYTE 0		// 1 byte length
#define WORDLEN_2BYTE 1		// 2 bytes length
#define _Y_ 0
#define _CBCR_ 1
#define _Y 0
#define _CB 1
#define _CR 2


static const UINT8 ZigZag_order2D[8][8] = { { 0, 1, 5, 6, 14, 15, 27, 28 }, {
		2, 4, 7, 13, 16, 26, 29, 42 }, { 3, 8, 12, 17, 25, 30, 41, 43 }, { 9,
		11, 18, 24, 31, 40, 44, 53 }, { 10, 19, 23, 32, 39, 45, 52, 54 }, { 20,
		22, 33, 38, 46, 51, 55, 60 }, { 21, 34, 37, 47, 50, 56, 59, 61 }, { 35,
		36, 48, 49, 57, 58, 62, 63 } };
static const UINT8 ZigZag_order[BLK_LENGTH] =
		{ 0, 1, 5, 6, 14, 15, 27, 28, 2, 4, 7, 13, 16, 26, 29, 42, 3, 8, 12,
				17, 25, 30, 41, 43, 9, 11, 18, 24, 31, 40, 44, 53, 10, 19, 23,
				32, 39, 45, 52, 54, 20, 22, 33, 38, 46, 51, 55, 60, 21, 34, 37,
				47, 50, 56, 59, 61, 35, 36, 48, 49, 57, 58, 62, 63 };
static const UINT8 Natural_order2D[8][8] = { { 0, 1, 8, 16, 9, 2, 3, 10 }, {
		17, 24, 32, 25, 18, 11, 4, 5 }, { 12, 19, 26, 33, 40, 48, 41, 34 }, {
		27, 20, 13, 6, 7, 14, 21, 28 }, { 35, 42, 49, 56, 57, 50, 43, 36 }, {
		29, 22, 15, 23, 30, 37, 44, 51 }, { 58, 59, 52, 45, 38, 31, 39, 46 }, {
		53, 60, 61, 54, 47, 55, 62, 63 } };
static const UINT8 Natural_order[BLK_LENGTH + 16] = { 0, 1, 8, 16, 9, 2, 3, 10,
		17, 24, 32, 25, 18, 11, 4, 5, 12, 19, 26, 33, 40, 48, 41, 34, 27, 20,
		13, 6, 7, 14, 21, 28, 35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23,
		30, 37, 44, 51, 58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55,
		62, 63, 63, 63, 63, 63, 63, 63, 63, 63, /* extra entries for safety in decoder */
		63, 63, 63, 63, 63, 63, 63, 63 };

static const UINT8 KochZhaoZZ_order[8] = { 3, 4, 5, 6, 7, 8, 12, 13 };

#endif /* DATATYPES_H_ */
