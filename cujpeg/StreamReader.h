/*
 * JPEGreader.h
 *
 *  Created on: 05.12.2011
 *      Author: id23cat
 */

#ifndef JPEGREADER_H_
#define JPEGREADER_H_
#include <stdio.h>
#include "Exceptions.h"

class StreamReader {
	FILE *file;
	char *fileName;
	long long filelength;
public:
	bool enable_exceptions;
private:
	size_t fileLength()throw ();
public:
	StreamReader() :
		file(NULL), fileName(NULL), filelength(-1), enable_exceptions(false) {
	}

	StreamReader(FILE *f) :
		file(f), fileName(NULL), filelength(-1), enable_exceptions(false) {
	}

	StreamReader(char *fname) throw (io_fail):
		file(NULL), fileName(NULL), filelength(-1), enable_exceptions(false)  {
		file = fopen(fname, "rb");
		if (!file) {
			throw io_fail(__FILE__, __LINE__,
							"StreamReader::OpenFile(): Can't open file ")
							<< str_info(fname);
		}
		fileLength();
	}

	~StreamReader() {
		CloseFile();
	}

	size_t Read(void *ptr, size_t bytes, size_t count) throw(io_fail);
	size_t mvForward(size_t bytes)throw (io_fail);			// move file pointer on bytes forward
	size_t mvBackward(size_t bytes)throw (io_fail);		// move file pointer on bytes backward
	long long FileLength() {
		return filelength;
	}
	long FileRest()throw ();

	int feof(){return ::feof(file);};
	int OpenFile(char *fname)throw (io_fail);
	int CloseFile() throw (io_fail);
};

#endif /* JPEGREADER_H_ */
