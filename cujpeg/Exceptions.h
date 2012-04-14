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
//#define DEBUG

#ifdef DEBUG
#include <signal.h>
#include <execinfo.h>
#include <cxxabi.h>
#include <dlfcn.h>
#include <stdlib.h>
#endif

//static char str_err[256];
//std::string str_err;
#define DEBUG_INFO(strout, file, line, str) sprintf(strout, "Error at %s:%d: %s" , file , line, str);


#define SAFE_MALLOC(ptr, count, TYPE) if(!(ptr = (TYPE*) malloc(count * sizeof(TYPE)))){\
			char str[256];\
			sprintf(str, "malloc(%d) -- failed", count * sizeof(TYPE));\
			throw memory_fail(__FILE__, __LINE__, str);}

#define SAFE_MALLOC_INT16(ptr, count) if(!(ptr = (INT16*) malloc(count * sizeof(INT16)))){\
			char str[256];\
			sprintf(str, "malloc(%d) -- failed", count * sizeof(INT16));\
			throw memory_fail(__FILE__, __LINE__, str);}

#define SAFE_MALLOC_UINT16(ptr, count) if(!(ptr = (UINT16*) malloc(count * sizeof(UINT16)))){\
			char str[256];\
			sprintf(str, "malloc(%ld) -- failed", count * sizeof(UINT16));\
			throw memory_fail(__FILE__, __LINE__, str);}

#define SAFE_MALLOC_INT8(ptr, count) if(!(ptr = (INT8*) malloc(count * sizeof(INT8)))){\
			char str[256];\
			sprintf(str, "malloc(%ld) -- failed", count * sizeof(INT8));\
			throw memory_fail(__FILE__, __LINE__, str);}

#define SAFE_MALLOC_UINT8(ptr, count) if(!(ptr = (UINT8*) malloc(count * sizeof(UINT8)))){\
			char str[256];\
			sprintf(str, "malloc(%d) -- failed", count * sizeof(UINT8));\
			throw memory_fail(__FILE__, __LINE__, str);}

#define SAFE_MALLOC_CHAR(ptr, count) if(!(ptr = (char*) malloc(count * sizeof(char)))){\
			char str[256];\
			sprintf(str, "malloc(%d) -- failed", count * sizeof(char));\
			throw memory_fail(__FILE__, __LINE__, str);}

#define SAFE_FREE(ptr) if(ptr) free(ptr);

#define CALL_STACK(strmes)using namespace abi;\
    enum{ MAX_DEPTH = 10 };\
    void *trace[MAX_DEPTH];\
    Dl_info dlinfo;\
    int status;\
    const char *symname;\
    char *demangled;\
    int trace_size = backtrace(trace, MAX_DEPTH);\
    sprintf(strmes, "\nCall stack: \n");\
    for (int i=0; i<trace_size; ++i){\
        if(!dladdr(trace[i], &dlinfo)) continue;\
        symname = dlinfo.dli_sname;\
        demangled = __cxa_demangle(symname, NULL, 0, &status);\
        if(status == 0 && demangled) symname = demangled;\
        sprintf(strmes,"%sobject: %s, function: %s\n", strmes, dlinfo.dli_fname, symname);\
        if (demangled) free(demangled);\
    }

#define TRY_CATCH_THROW(X_METHOD) try{X_METHOD;}catch(my_exception &exc)\
		{ exc<<str_info("From ")<<str_info(__FILE__)<<str_info(":")\
	<<int_info(__LINE__)<<str_info(" (")<<str_info(/*__FUNCTION__*/__PRETTY_FUNCTION__)<<\
			str_info(")"); throw exc;}

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
	virtual ~my_exception()throw(){};
	virtual const char* what() const throw() {return mess;};
};

struct io_fail: virtual my_exception {
public:
	io_fail(std::string str):my_exception(str){};
	io_fail(std::string file, int line, std::string str):my_exception(file, line, str){};
	virtual ~io_fail()throw(){};
};

struct memory_fail: virtual my_exception {
public:
	memory_fail(std::string str):my_exception(str){};
	memory_fail(std::string file, int line, std::string str):my_exception(file, line, str){};
	virtual ~memory_fail()throw(){};
};

struct NULLptr_fail: virtual my_exception {
public:
	NULLptr_fail(std::string str):my_exception(str){};
	NULLptr_fail(std::string file, int line, std::string str):my_exception(file, line, str){};
	virtual ~NULLptr_fail()throw(){};
};

struct indexing_fail: virtual my_exception {
public:
	indexing_fail(std::string str):my_exception(str){};
	indexing_fail(std::string file, int line, std::string str):my_exception(file, line, str){};
	virtual ~indexing_fail()throw(){};
};

struct unknown_type_fail: virtual my_exception {
public:
	unknown_type_fail(std::string str):my_exception(str){};
	unknown_type_fail(std::string file, int line, std::string str):my_exception(file, line, str){};
	virtual ~unknown_type_fail()throw(){};
};

struct tree_fail: virtual my_exception {
public:
	tree_fail(std::string str):my_exception(str){};
	tree_fail(std::string file, int line, std::string str):my_exception(file, line, str){};
	virtual ~tree_fail()throw(){};
};

#endif /* EXCEPTIONS_H_ */
