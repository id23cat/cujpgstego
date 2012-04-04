/*
 * cujpeg.cpp
 *
 *  Created on: 02.12.2011
 *      Author: id23cat
 */
//#include <stdio.h>
#include "JPEGfile.h"
#include "KZanalizer.h"
//#define FNAME "g.jpg"
//#include "histogramtest.hpp"
//#define FNAME "005.jpg"
//#define FNAME "106.jpg"
#define FNAME "11-out.jpg"

//#define LOG "106.log"
#define LOG "11-out.log"


int main(int argc, char **argv) {
	try {
		JPEG jpeg((char*) FNAME); // create jpeg object for a file
		jpeg.GetDCTs(); // get DCT coefficients from file
		std::cout << "Read is OK\n";
		if (!jpeg.cmpWith((char*) LOG)) { // compare DCTs with existing
			std::cout << "Not equal\n";
			return 1;
		}
//		jpeg.PrintData();
		std::cout << "All is OK\n";

		KZanalizer kz(jpeg.begin(), jpeg.end());

	} catch (my_exception exc) {
		std::cerr << exc.what() << std::endl;
		if ( int const * mi=boost::get_error_info<int_info>(exc) )
			std::cerr << *mi << std::endl;
		if ( std::string const * ms=boost::get_error_info<str_info>(exc) )
			std::cerr << *ms << std::endl;
		return errno;
	} catch (...){
		std::cerr << "Unknown exception" << std::endl;
	}

//	histtest();

	return 0;
}
