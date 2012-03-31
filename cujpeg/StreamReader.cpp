/*
 * JPEGreader.cpp
 *
 *  Created on: 05.12.2011
 *      Author: id23cat
 */

#include <string.h>
#include <stdlib.h>
#include <boost/exception/all.hpp>
#include <stdexcept>
#include "StreamReader.h"

//////// private ////////////
size_t StreamReader::fileLength() throw (){
	long bp = ftell(file);
	fseek(file, 0, SEEK_END);
	long ep = ftell(file);
	fseek(file, 0, SEEK_SET);

	filelength = ep - bp;
	return filelength;
}

////////// public ///////////
size_t StreamReader::Read(void *ptr, size_t bytes, size_t count)
		throw (io_fail) {
	size_t ret = fread(ptr, bytes, count, file);
	if (ret < count && enable_exceptions) {
		throw io_fail(__FILE__, __LINE__,
				"StreamReader::Read(): Can't read file ") << str_info(fileName);
	}
	return ret;
}

size_t StreamReader::mvForward(size_t bytes) throw (io_fail){
	if (fseek(file, bytes, SEEK_CUR)<0){
		if (enable_exceptions)
			throw io_fail(__FILE__, __LINE__, "StreamReader::mvForward(): ")
					<< str_info(strerror(errno)) << str_info(fileName);
		else
			return 0;
	}
	return bytes;
}

size_t StreamReader::mvBackward(size_t bytes)throw (io_fail){
	if (fseek(file, -bytes, SEEK_CUR)<0){
		if (enable_exceptions)
			throw io_fail(__FILE__, __LINE__, "StreamReader::mvBackward(): ")
					<< str_info(strerror(errno)) << str_info(fileName);
		else
			return 0;
	}
	return bytes;
}

int StreamReader::OpenFile(char *fname) throw (io_fail){
	file = fopen(fname, "rb");
	if (!file) {
		throw io_fail(__FILE__, __LINE__,
				"StreamReader::OpenFile(): Can't open file ")
				<< str_info(fname);
	}

	fileName = std::string(fname);

//	SAFE_MALLOC_CHAR(fileName, strlen(fname) + 1);
//	strcpy(fileName, fname);

	return fileLength();;
}

int StreamReader::CloseFile() throw (io_fail){
	if (!file && fileName.empty()) {
		std::cerr << "File " << fileName << " not opened" << std::endl;
		if (enable_exceptions)
			throw io_fail(__FILE__, __LINE__,
					"StreamReader::CloseFile(): File not openend: ")
					<< str_info(fileName);
		return -1;
	}
	if (file)
		fclose(file);
	file = NULL;
//	if (fileName)
//		free(fileName);
//	fileName = NULL;

	return 0;
}

long StreamReader::FileRest() throw (){
	long bp = ftell(file);
	return filelength - bp;
}

