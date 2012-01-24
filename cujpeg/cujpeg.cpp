/*
 * cujpeg.cpp
 *
 *  Created on: 02.12.2011
 *      Author: id23cat
 */
//#include <stdio.h>
#include "JPEGfile.h"
//#define FNAME "g.jpg"
//#define FNAME "005.jpg"
//#define FNAME "106.jpg"

int main(int argc, char **argv)
{
	try{
	JPEG jpeg((char*)FNAME);
	jpeg.GetDCTs();
	std::cout << "Read is OK\n";
	if(!jpeg.cmpWith((char*)"106.log")){
		std::cout << "Not equal\n";
		return 1;
	}
	std::cout << "All is OK\n";

	}catch(my_exception exc){
		std::cerr << exc.what() <<endl;
		 if( int const * mi=boost::get_error_info<int_info>(exc) )
			 std::cerr << *mi << endl;
		 if( std::string const * ms=boost::get_error_info<str_info>(exc) )
			 std::cerr << *ms <<endl;
		return errno;
	}
	return 0;
}
