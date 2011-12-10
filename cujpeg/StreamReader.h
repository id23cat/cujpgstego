/*
 * JPEGreader.h
 *
 *  Created on: 05.12.2011
 *      Author: id23cat
 */

#ifndef JPEGREADER_H_
#define JPEGREADER_H_
#include <stdio.h>

class StreamReader {
	FILE *file;
	char *fileName;
	long long filelength;
public:
	bool enable_exceptions;
private:
	size_t fileLength();
public:
	StreamReader() :
		file(NULL), fileName(NULL), filelength(-1), enable_exceptions(false) {
	}

	StreamReader(FILE *f) :
		file(f), fileName(NULL), filelength(-1), enable_exceptions(false) {
	}

	StreamReader(char *fname) :
		file(NULL), fileName(NULL), filelength(-1), enable_exceptions(false)  {
		file = fopen(fname, "rb");
		if (!file) {
			fprintf(stderr, "Can not open file %s", fname);
			throw 1;
		}
		fileLength();
	}

	~StreamReader() {
		CloseFile();
	}

	size_t Read(void *ptr, size_t bytes, size_t count);
	long long FileLength() {
		return filelength;
	}
	long FileRest();

	int feof(){return ::feof(file);};
	int OpenFile(char *fname);
	int CloseFile();
};

#endif /* JPEGREADER_H_ */
