/*
 * Exceptions.h
 *
 *  Created on: 18.01.2012
 *      Author: id23cat
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_
#include <boost/exception/all.hpp>
#include <stdexcept>
//#include <ios_base.h>

//static char str_err[256];
//std::string str_err;
#define DEBUG_INFO(strout, file, line, str) sprintf(strout, "Error at %s:%d: %s" , file , line, str);

typedef boost::error_info<struct int_my_info, int> int_info;
typedef boost::error_info<struct str_my_info, std::string> str_info;


struct my_exception: virtual boost::exception, virtual std::exception {
protected:
	char mess[256];
public:
	my_exception(std::string str){sprintf(mess,"%s", str.data());};
	my_exception(std::string file, int line, std::string str){
		DEBUG_INFO(mess, file.data(), line, str.data());
	};
	virtual const char* what() const throw() {return mess;};
};

struct io_fail: virtual my_exception {
public:
	io_fail(std::string str):my_exception(str){};
	io_fail(std::string file, int line, std::string str):my_exception(file, line, str){};
};

struct memory_fail: virtual my_exception {
public:
	memory_fail(std::string str):my_exception(str){};
	memory_fail(std::string file, int line, std::string str):my_exception(file, line, str){};
};

struct indexing_fail: virtual my_exception {
public:
	indexing_fail(std::string str):my_exception(str){};
	indexing_fail(std::string file, int line, std::string str):my_exception(file, line, str){};
};

struct unknown_type_fail: virtual my_exception {
public:
	unknown_type_fail(std::string str):my_exception(str){};
	unknown_type_fail(std::string file, int line, std::string str):my_exception(file, line, str){};
};

#endif /* EXCEPTIONS_H_ */
