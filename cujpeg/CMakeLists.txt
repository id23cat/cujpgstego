cmake_minimum_required (VERSION 2.6)

set (PROJECT stegAnalizer)
project (${PROJECT})

set (HEADERS datatypes.h DCTdataIterator.h Exceptions.h JPEGfile.h KZanalizer.h StreamReader.h huftree.h)
set (SOURCES cujpeg.cpp DCTdataIterator.cpp JPEGfile.cpp KZanalizer.cpp StreamReader.cpp huftree.cpp)
add_executable (${PROJECT} ${HEADERS} ${SOURCES})
