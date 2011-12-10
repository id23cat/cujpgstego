/*
 * readJPEG.cpp
 *
 *  Created on: 02.12.2011
 *      Author: id23cat
 */
#include <stdio.h>
#include <stdlib.h>
#include "readJPEG.h"


size_t getFileLength(FILE *jfile) {
	long bp = ftell(jfile);
	fseek(jfile, 0, SEEK_END);
	long ep = ftell(jfile);
	fseek(jfile, 0, SEEK_SET);

	return (ep - bp);
}

template<typename T>
T readFile(FILE *jfile, size_t count) {
	unsigned char *c = (unsigned char*) malloc(count * sizeof(T));
	T t;
	fread(c, count, sizeof(c), jfile);
	t = (c[0] << 8) | c[1];
	free(c);
	return t;
}

UINT16 readFlag(FILE *jfile) {
	char c[2];
	UINT16 t;
	fread(c, 2, 1, jfile);
	t = (c[0] << 8) | c[1];
	return t;
}

int readComment(FILE *jfile, struct JPEG *jpeg)
{
	struct JComment *comm = &(jpeg->comment);
	comm->length = readFlag(jfile) - 2;
	comm->str = (char*)malloc(comm->length);
	if(fread(comm->str, 1, comm->length, jfile)<comm->length)
			return -1;
	return 0;

}

inline UINT8 low(UINT8 byte)
{
	return (byte & 0x0F);
}

inline UINT8 high(UINT8 byte)
{
	return (byte >> 4);
}

template<typename T>
int fillZigZag(T M[8][8], T *v){
	for(int i=0; i<8; i++)
		for(int j=0; j<8; j++)
			M[i][j] = v[ZigZag[i][j]];
	return 0;
}

//int readDQT(FILE *jfile, struct JPEG *jpeg){
//	struct DQT *qt = &(jpeg->qTable);
//	qt->length = readFlag(jfile) - 3;
//	UINT8 byte;
//	fread(&byte, 1, 1, jfile);
//	qt->tElLength = high(byte);
//	qt->qtID = low(byte);
//	if(qt->tElLength == WORDLEN_1BYTE){
//		UINT8 temp[64];
//		if(fread(temp, sizeof(UINT8), qt->length, jfile))
//	}
//	else{
//
//	}
//
//}

void deleteJPEG(struct JPEG *jpeg){
	if(jpeg->comment.str)
		free(jpeg->comment.str);

}

int parseJpeg(FILE *jfile) {
	UINT16 flag;
	struct JPEG jpeg;
	while (!feof(jfile)) {
		flag = readFlag(jfile);

		switch (flag) {
		case 0xFFD8:			//	begin
			printf("FFD8\n");
			break;
		case 0xFFFE:			// comment
			printf("FFFE\n");
			readComment(jfile, &jpeg);
			printf("Comment: %s\n", jpeg.comment.str);
			break;
		case 0xFFDB:			// DQT
			printf("FFDB");
			break;
		case 0xFFC0:			// SOF0
			printf("FFC0");
			break;
		case 0xFFC4:			// DHT
			printf("FFC4");
			break;
		case 0xFFDA:			// SOS
			printf("FFDA");
			break;
		case 0xFFD9:			// EOF
			printf("FFD9");
			deleteJPEG(&jpeg);
			return 0;
		default:
			deleteJPEG(&jpeg);
			return -1;
		}
	}
	deleteJPEG(&jpeg);
	return -1;
}
