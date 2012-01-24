/*
 * JPEGfile.cpp
 *
 *  Created on: 05.12.2011
 *      Author: id23cat
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "JPEGfile.h"
#include "DCTdataIterator.h"

//#define PRINT_DATA
//#define PRINT_DCT_DECODE_PROCESS
//#define PRINT_TREE
#define PRINT_CMP_PROCESS

#define base0(ID) (ID-1)

inline void DEBUG_MSG(char *str) {
#ifdef DEBUG
	printf(str);
#endif
}

inline UINT16 h2l(UINT16 word) {
	//	//UINT8 *ptr = static_cast<UINT8*>(&word);
	//	UINT16 hi = (word & 0xFF00)>>8;
	//	UINT16 lo = (word & 0x00FF)<<8;
	//	UINT16 ret = lo|hi;
	return (word & 0xFF00) >> 8 | (word & 0x00FF) << 8;
}

int SOF0::memmv1bLeft(UINT8 *start, int len){
	for(int i=1; i<len; i++)
		start[i-1] = start[i];
	return len;
}
JPEG::~JPEG() {
	if (comment.str)
		free(comment.str);
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			if (hTable[i][j].code && hTable[i][j].mcodeslength) {
				free(hTable[i][j].code);
				hTable[i][j].mcodeslength = 0;
			}
	if (data.srcDataPtr)
		free(data.srcDataPtr);
	if (DCTdata)
		free(DCTdata);
}

JPEG::JPEG(char *jfname) {
	data.srcDataPtr = NULL;
	jfile.OpenFile(jfname);
	jfile.enable_exceptions = false;
	comment.length = 0;
	comment.str = NULL;
	qTable[_Y_].length = 0;
	qTable[_CBCR_].length = 0;
	hTable[_Y_][_DC].length = 0;
	hTable[_Y_][_AC].length = 0;
	hTable[_CBCR_][_DC].length = 0;
	hTable[_CBCR_][_AC].length = 0;
	DCTdata = NULL;

	parseJpeg();
	jfile.CloseFile();
}

UINT16 JPEG::readFlag() {
	UINT16 t;
	jfile.Read(&t, 2, 1);
	//t = (c[0] << 8) | c[1];
	t = h2l(t);
	return t;
}

int JPEG::readJFIF() {
	size_t length = readFlag();
	//	UINT8 *t = (UINT8*)malloc(length-2);
	//	jfile.Read(t, 1, length-2);
	//#ifdef PRINT_DATA
	//	for(int i=0; i<length-2; i++)	printf("%x ", t[i]);
	//	printf("\n");
	//#endif
	////	jfile.Read(&jfif.id, 1, 5);
	////	jfile.Read(&jfif.version, 2, 1);
	////	jfile.Read(&jfif.units, 1, 1);
	////	jfile.Read(&jfif.xDensity, 2, 1);
	////	jfile.Read(&jfif.yDensity, 2, 1);
	////	jfile.Read(&jfif.xThumbnail, 1, 1);
	////	jfile.Read(&jfif.yThumbnail, 1, 1);
	////	jfile.Read(jfif.n, 3, 1);
	//	free(t);
	jfile.mvForward(length - 2);
	return 0;
}

int JPEG::readComment() {
	comment.length = readFlag() - 2; // read length
	comment.str = (char*) malloc(comment.length);
	if (jfile.Read(comment.str, 1, comment.length) < comment.length)
		return -1;
	return 0;

}

int JPEG::readDQT() {
	size_t length = readFlag(); // read length
	UINT8 byte;

	jfile.Read(&byte, 1, 1);
	UINT8 ID = LOW(byte);
	qTable[ID].qtID = ID;
	qTable[ID].length = length;
	qTable[ID].tElLength = HIGH(byte);

	if (qTable[ID].tElLength == WORDLEN_1BYTE)
		jfile.Read(qTable[ID].qTable8, 1, 64);
	else
		jfile.Read(qTable[ID].qTable16, 1, 64);

	//	if (ID == _Y_) {
	//		qTableY.qtID = ID;
	//		qTableY.length = length;
	//		qTableY.tElLength = HIGH(byte);
	//
	//		if (qTableY.tElLength == WORDLEN_1BYTE)
	//			jfile.Read(qTableY.qTable8, 1, 64);
	//		else
	//			jfile.Read(qTableY.qTable16, 1, 64);
	//	} else {
	//		qTableCbCr.qtID = ID;
	//		qTableCbCr.length = length;
	//		qTableCbCr.tElLength = HIGH(byte);
	//
	//		if (qTableCbCr.tElLength == WORDLEN_1BYTE)
	//			jfile.Read(qTableCbCr.qTable8, 1, 64);
	//		else
	//			jfile.Read(qTableCbCr.qTable16, 1, 64);
	//	}
	return 0;
}

int JPEG::readSOF0() {
	data.length = readFlag(); // read length

	jfile.Read(&data.precision, 1, 1);
	UINT16 u16;
	jfile.Read(&u16, 1, 2);
	data.imHeight = h2l(u16);
	jfile.Read(&u16, 1, 2);
	data.imWidth = h2l(u16);
	jfile.Read(&data.componentsCount, 1, 1);
	// Components
	UINT8 u8;
	data.maxHDecimation = data.maxVDecimation = 0;
	for (int i = 0; i < data.componentsCount; i++) {
		jfile.Read(&u8, 1, 1);

		data.component[i].cID = u8;
		jfile.Read(&u8, 1, 1);
		data.component[i].horizontalDecimation = HIGH(u8);
		if (data.maxHDecimation < data.component[i].horizontalDecimation)
			data.maxHDecimation = data.component[i].horizontalDecimation;
		data.component[i].verticalDecimation = LOW(u8);
		if (data.maxVDecimation < data.component[i].verticalDecimation)
			data.maxVDecimation = data.component[i].verticalDecimation;
		jfile.Read(&data.component[i].qtID, 1, 1);
		//		data.component[i].

		//		switch (u8) {
		//		case _Y:
		//			data.Y.cID = u8;
		//			jfile.Read(&u8, 1, 1);
		//			data.Y.horizontalDecimation = HIGH(u8);
		//			data.Y.verticalDecimation = LOW(u8);
		//			jfile.Read(&data.Y.qtID, 1, 1);
		//			break;
		//
		//		case _CB:
		//			data.Cb.cID = u8;
		//			jfile.Read(&u8, 1, 1);
		//			data.Cb.horizontalDecimation = HIGH(u8);
		//			data.Cb.verticalDecimation = LOW(u8);
		//			jfile.Read(&properties.Cb.qtID, 1, 1);
		//			break;
		//		case _CR:
		//			properties.Cr.cID = u8;
		//			jfile.Read(&u8, 1, 1);
		//			properties.Cr.horizontalDecimation = HIGH(u8);
		//			properties.Cr.verticalDecimation = LOW(u8);
		//			jfile.Read(&properties.Cr.qtID, 1, 1);
		//			break;
		//		}
	}
	return 0;
}

int JPEG::readDHT() {
	UINT16 length = readFlag(); // read length
	UINT8 u8;
	jfile.Read(&u8, 1, 1); // read Class & ID
	UINT8 cls = HIGH(u8);
	UINT8 ID = LOW(u8);
	hTable[ID][cls].length = length;
	hTable[ID][cls].htID = ID;
	hTable[ID][cls].cls = cls;
	jfile.Read(hTable[ID][cls].codeCount, 1, 16);

	UINT8 count = length - 2 - 1 - 16;
	hTable[ID][cls].code = (UINT8*) malloc(count);
	hTable[ID][cls].mcodeslength = count;
	jfile.Read(hTable[ID][cls].code, 1, count);
	hTable[ID][cls].CalcTree();
	return 0;
}

void DHT::CalcTree() {
	int k = 0;
	for (int i = 0; i < 16; i++) {
		for (int j = codeCount[i]; j > 0; j--) {
			if (j) {
				treeLIST list;
				list.codelength = i + 1;
				list.code = code[k];
				tree.AddList(list);
				k++;
				if (k > mcodeslength)
					return;
			}
		}
	}
}

int JPEG::readSOS() {
	data.length = readFlag(); // read length
	jfile.Read(&data.componentsCount, 1, 1);
	UINT8 ID;
	UINT8 u8;
	for (int i = 0; i < data.componentsCount; i++) {
		jfile.Read(&ID, 1, 1);
		data.component[base0(ID)].cID = ID;
		jfile.Read(&u8, 1, 1);
		data.component[base0(ID)].hufDCID = HIGH(u8);
		data.component[base0(ID)].hufDC = &(hTable[HIGH(u8)][_DC]);
		data.component[base0(ID)].hufACID = LOW(u8);
		data.component[base0(ID)].hufAC = &(hTable[LOW(u8)][_AC]);
	}

	UINT8 t[3];
	jfile.Read(t, 1, 3);
	data.srcDataLength = jfile.FileRest() - 2;
	data.srcDataPtr = (UINT8*) malloc(data.srcDataLength);
	jfile.Read(data.srcDataPtr, 1, data.srcDataLength);
	for (size_t i = 0; i < data.srcDataLength - 1; i++) {
		if (data.srcDataPtr[i] == 0xFF && data.srcDataPtr[i + 1] == 0x00) {
#ifdef PRINT_DATA
			printf("FIND FF001 in data!\n");
#endif
			data.memmv1bLeft(&data.srcDataPtr[i], data.srcDataLength - i);
//			memmove(data.srcDataPtr + i + 1, data.srcDataPtr + i + 2,
//					data.srcDataLength - i + 2);
			data.srcDataLength--;
		}
	}
	return 0;
}

int JPEG::parseJpeg() {
	UINT16 flag;
	while (!jfile.feof()) {
		flag = readFlag();

		switch (flag) {
		case 0xFFD8: //	begin
			printf("FFD8\n");
			break;
		case 0xFFE0: //	JFIF
		case 0xFFE1: //	EXIF
		case 0xFFE2: //	ICC
		case 0xFFE3: //	JPS
		case 0xFFE4: //
		case 0xFFE5: //
		case 0xFFE6: //	NITF
		case 0xFFE7: //
		case 0xFFE8: //
		case 0xFFE9: //
		case 0xFFEA: //	ActiveObject
		case 0xFFEB: //	HELIOS JPEG Resources
		case 0xFFEC: //	Picture Info
		case 0xFFED: //	Photoshop
		case 0xFFEE: //
		case 0xFFEF: //
			printf("FFEx\n");
			readJFIF();
			//			jfif.PrintData();
			break;
		case 0xFFFE: // comment
			printf("FFFE\n");
			readComment();
#ifdef PRINT_DATA
			comment.PrintData();
#endif
			break;
		case 0xFFDB: // DQT
			printf("FFDB\n");
			readDQT();
#ifdef PRINT_DATA
			//			qTableY.PrintData();
			//			qTableCbCr.PrintData();
			qTable[0].PrintData();
			qTable[1].PrintData();
#endif
			break;
		case 0xFFC0: // SOF0
			printf("FFC0\n");
			readSOF0();
#ifdef PRINT_DATA
			data.PrintData();
#endif
			break;
		case 0xFFC4: // DHT
			printf("FFC4\n");
			readDHT();
#ifdef PRINT_DATA
			hTable[_Y_][_DC].PrintData();
			hTable[_Y_][_AC].PrintData();
			hTable[_CBCR_][_DC].PrintData();
			hTable[_CBCR_][_AC].PrintData();
#endif
#ifdef PRINT_TREE
			//			hTable[_Y_][_DC].tree.PrintData();
			hTable[_Y_][_AC].tree.PrintData();
			//			hTable[_CBCR_][_DC].tree.PrintData();
			//			hTable[_CBCR_][_AC].tree.PrintData();
#endif
			break;
		case 0xFFDA: // SOS
			printf("FFDA\n");
			readSOS();
#ifdef PRINT_DATA
			data.PrintData();
			printf("\nLength of data: %d\nDATA: ", (int) data.srcDataLength);
			for (size_t i = 0; i < data.srcDataLength; i++) {
				printf("%x ", data.srcDataPtr[i]);
			}
			printf("\n");
#endif
			break;
		case 0xFFD9: // EOF
			printf("FFD9\n");
			return 0;
		default:
			//			fprintf(stderr, "ooo!: Unknown JPEG flag: 0x%X\n", flag);
			std::stringstream sstr;
			sstr << std::hex << flag;
			throw unknown_type_fail(__FILE__, __LINE__, "Unknown JPEG flag: ")
					<< str_info(sstr.str());

			return -1;
		}
	}
	//deleteJPEG(&jpeg);
	return 0;
}

void JPEG::GetDCTs() {
	try {
		BITSETiterator bit(data.srcDataPtr, data.srcDataLength);
		DCTdataLength = 0;
		for (int i = 0; i < data.componentsCount; i++)
			DCTdataLength += data.imWidth
					* ((float) data.component[i].horizontalDecimation
							/ (float) data.maxHDecimation) * data.imHeight
					* ((float) data.component[i].verticalDecimation
							/ (float) data.maxVDecimation);
		//		DCTdata = (INT16*) calloc(DCTdataLength, sizeof(INT16));
		//		DCTdataLength *= sizeof(INT16);
		DCTdata = (INT16*) malloc(DCTdataLength * sizeof(INT16));

		//#ifdef PRINT_DCT_DECODE_PROCESS
		printf("!! Alloc %d bytes\n", (int) DCTdataLength);
		//#endif
		//		DCTdataLength = data.imWidth * data.imHeight * data.componentsCount * sizeof(INT16);

		memset(DCTdata, 1, DCTdataLength);
		DCTdataIterator DCTs(DCTdata, DCTdataLength, data);

		int dctIdx;
		UINT8 c0, c1;//, clen;
		//for (int i = _Y_; i <= _CBCR_; i++) {
		int blk_counter = 0;
		int blkCount = DCTdataLength / (64 * 2);

		while (blk_counter < blkCount) {
#ifdef PRINT_DCT_DECODE_PROCESS
			{
//				printf("blkCount = %d, counter = %d\n", blkCount, blk_counter);
//				PrintData(blkCount);
			}
#endif
			dctIdx = 0;
			//for(int j=_DC; j<=_AC; j++){
			int i = (DCTs.color() > 0);

			// calculating of DC coefficient
			{
				hTable[i][_DC].tree.ResetCurrentPointer();
				while (!hTable[i][_DC].tree.MovePtr(bit.GetBit()))
					bit.NextBit();
				//				printf("\n");
				UINT8 clen = c0 = hTable[i][_DC].tree.GetCode();
#ifdef PRINT_DCT_DECODE_PROCESS
				printf("=== Block#: %d ===\n", blk_counter);
				printf("c0=%d\n", c0);
#endif
				if (!c0) {
					bit.NextBit();
					DCTs[dctIdx++] = (INT16) c0 + DCTs.getPrevDC();
				} else {
					c1 = bit.NextBit().GetBit();
					bool nativeValue = (bool) c1;
					c0--;
					while (c0) {
						//						c1 = c1<<1;
						c1 = (c1 << 1) | bit.NextBit().GetBit();
						c0--;
					}
#ifdef PRINT_DCT_DECODE_PROCESS
					//					printf("\n");
					printf("c1=%d\n", c1);
#endif

					bit.NextBit();
					if (nativeValue)
						DCTs[dctIdx++] = c1 + DCTs.getPrevDC();
					else {
						//						INT16 t = c1 - (1 << clen) + 1 + DCTs.getPrevDC();
						//						DCTs[dctIdx++] = t;
						DCTs[dctIdx++] = c1 - (2 << (clen - 1)) + 1
								+ DCTs.getPrevDC();
					}
				}
			}

			// calculating of AC coefficients

			while (dctIdx < 64) {
				//				bit.PrintData();
				hTable[i][_AC].tree.ResetCurrentPointer();
#ifdef PRINT_DCT_DECODE_PROCESS
				//				UINT8 b;
				//				bool l;
				//				do{
				//					b = bit.GetBit();
				//					l = hTable[i][_AC].tree.MovePtr(b);
				//					bit.NextBit();
				//				}while (!l);
#endif
				while (!hTable[i][_AC].tree.MovePtr(bit.GetBit()))
					bit.NextBit();
				c0 = hTable[i][_AC].tree.GetCode();
				if (!c0) {
					while (dctIdx < 64)
						DCTs[dctIdx++] = 0;
					continue;
				}

				UINT8 zerocount = c0 >> 4;
				while (dctIdx < 64 && zerocount) {
					DCTs[dctIdx++] = 0;
					//					printf(" %d ", DCTs[dctIdx-1]);
					zerocount--;
				}

				UINT8 clen = c0 & 0x0F;
				if (clen && dctIdx < 64) {
					c1 = bit.NextBit().GetBit();
					bool nativeValue = (bool) c1;
					clen--;
					while (clen) {
						//						c1 = c1<<1;
						c1 = (c1 << 1) | bit.NextBit().GetBit();
						clen--;
					}

					if (nativeValue)
						DCTs[dctIdx++] = c1;
					else {
						//						INT16 q1 = c0 & 0x0F;
						//						INT16 q2 = 2 << (q1 - 1);
						//						INT16 q3 = c1 - q2 + 1;
						//						DCTs[dctIdx++] = q3;
						DCTs[dctIdx++] = c1 - (2 << ((c0 & 0x0F) - 1)) + 1;
					}
				}
				bit.NextBit();
			}
			bit.NextBit();
#ifdef PRINT_DCT_DECODE_PROCESS
			printf("\n");
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					printf("%d ", DCTs[ZigZag[i][j]]);
				}
				printf("\n");
			}
			printf("\n");
			DCTs.PrintData();
#endif
			blk_counter++;
			if (!DCTs.lastBlock())
				DCTs.mvToNextBlock();
			else {
#ifdef PRINT_DCT_DECODE_PROCESS
				for (int i = 0; i < blkCount; i++) {
					for (int j = 0; j < 64; j++) {
						printf("%d ", DCTdata[i * 64 + j]);
					}
					printf("\n");
				}
#endif
				return;
			}
		}
	} catch (int) {
		fprintf(stderr, "Exception int cached!\n");
	}
	//	} catch (...) {
	//		fprintf(stderr, "Exception ... cached!\n");
	//	}

}

bool JPEG::cmpWith(char *fname) {
	StreamReader cFile(fname);
	INT16 *cdat = (INT16*) malloc(DCTdataLength * sizeof(INT16));
	INT16 *curptr = cdat;
	INT16 t = 0;
	while (!cFile.feof()) {
		cFile.Read(&t, 2, 1);
		cFile.Read(curptr, sizeof(INT16), BLK_LENGTH);
		curptr += BLK_LENGTH;
	}
	DCTdataIterator it1(DCTdata, DCTdataLength, data);
	DCTdataIterator it2(cdat, DCTdataLength, data);
	int ret=0;

	int r=0;
	for (int i = 0, line=0; i < 90/*DCTdataLength / (64)*/; i++) {
#ifdef PRINT_CMP_PROCESS
		std::cout <<"\t"<< i<< ":: ";
#endif
		for (int j = 1; j < 64; j++, line++) {
#ifdef PRINT_CMP_PROCESS
			std::cout << (int) it1.ZigZagView(j) << ":" << (int) it2.LineView(j) << " ";
#endif
//			std::cout << (int) DCTdata[j] << ":" << (int) cdat[j] << " ";
			//		if(DCTdata[i] != cdat[i])
			//			return false;
			r += it1.ZigZagView(j) - it2.LineView(j);
			ret += r;
		}
#ifdef PRINT_CMP_PROCESS
		if(!r) std::cout << " +++OK";
		else std::cout << " ---FAIL";
		std::cout << std::endl;
#endif
		r=0;
		it1.mvToNextBlock();
		it2.mvToNextBlock();
	}
	free(cdat);
	if(ret) return false;
	return true;
}

void JPEG::PrintData(/*class DCTdataIterator &d,*/int count) {
	//	class DCTdataIterator t = d;
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < 64; j++) {
			printf("%d ", DCTdata[i * 64 + j]/*, t[j]*/);
		}
		printf("\n");
		//		t.NextBlock();
	}
}
