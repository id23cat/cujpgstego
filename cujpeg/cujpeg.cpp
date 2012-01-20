/*
 * cujpeg.cpp
 *
 *  Created on: 02.12.2011
 *      Author: id23cat
 */
//#include <stdio.h>
#include "JPEGfile.h"
//#define FNAME "g.jpg"
#define FNAME "005.jpg"

int main(int argc, char **argv)
{
	try{
//	FILE *f = fopen(FNAME, "rb");
//	if(!f){
//		fprintf(stderr, "Can not open file %s", FNAME);
//		return 1;
//	}
//	parseJpeg(f);
//	fclose(f);
	JPEG jpeg((char*)FNAME);
	jpeg.GetDCTs();
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
