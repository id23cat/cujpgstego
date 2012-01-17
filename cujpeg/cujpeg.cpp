/*
 * cujpeg.cpp
 *
 *  Created on: 02.12.2011
 *      Author: id23cat
 */
#include <stdio.h>
#include "JPEGfile.h"
#define FNAME "g.jpg"
//#define FNAME "005.jpg"

int main(int argc, char **argv)
{
//	FILE *f = fopen(FNAME, "rb");
//	if(!f){
//		fprintf(stderr, "Can not open file %s", FNAME);
//		return 1;
//	}
//	parseJpeg(f);
//	fclose(f);
	JPEG jpeg(FNAME);
	jpeg.GetDCTs();
	printf("All is OK\n");

}
