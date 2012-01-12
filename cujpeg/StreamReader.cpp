/*
 * JPEGreader.cpp
 *
 *  Created on: 05.12.2011
 *      Author: id23cat
 */

#include <string.h>
#include <stdlib.h>
#include "StreamReader.h"

//////// private ////////////
size_t StreamReader::fileLength() {
	long bp = ftell(file);
	fseek(file, 0, SEEK_END);
	long ep = ftell(file);
	fseek(file, 0, SEEK_SET);

	filelength = ep - bp;
	return filelength;
}

////////// public ///////////
size_t StreamReader::Read(void *ptr, size_t bytes, size_t count) {
	size_t ret = fread(ptr, bytes, count, file);
	if (ret < count && enable_exceptions)
		throw -1;

	return ret;
}

int StreamReader::OpenFile(char *fname) {
	file = fopen(fname, "rb");
	if (!file) {
		fprintf(stderr, "Can not open file %s", fname);
		throw 1;
	}
	fileName = (char*) malloc(strlen(fname)+1);
	strcpy(fileName, fname);
	fileLength();
}

int StreamReader::CloseFile() {
	if (!file && fileName) {
		fprintf(stderr, "File %s not openend", fileName);
		if (enable_exceptions)
			throw 1;
		return -1;
	}
	if(file)
		fclose(file);
	file=NULL;
	if(fileName)
		free(fileName);
	fileName = NULL;
}

long StreamReader::FileRest() {
	long bp = ftell(file);
	return filelength - bp;
}

