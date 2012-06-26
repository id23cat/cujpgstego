set (PROJECT KZjpeg)
project ( ${PROJECT} )
# Поддержка CUDA появилась в CMake 2.8
cmake_minimum_required(VERSION 2.8)


set ( SOURCES cujpeg.cpp DCTdataIterator.cpp huftree.cpp JPEGfile.cpp KZanalizer.cpp StreamReader.cpp )
set ( HEADERS cudefines.h Exceptions.h huftree.h DCTdataIterator.h JPEGfile.h KZanalizer.h StreamReader.h )
set ( CUDA_SOURCES KZanalizerCUDA.cu )
set ( CUDA_LIBRARIES cudart shrutil_x86_64 cutil_x86_64 )

find_package ( CUDA REQUIRED )
MESSAGE("CUDA")
IF(NOT CUDA_FOUND)
	MESSAGE("CUDA COULD NOT BE FOUND!")
ENDIF(NOT CUDA_FOUND)

message("CUDA_SDK_ROOT_DIR = ${CUDA_SDK_ROOT_DIR}")
message("CUDA_INCLUDE_DIRS = ${CUDA_INCLUDE_DIRS}")
message("CUDA_LIBRARIES = ${CUDA_LIBRARIES}")
message("CUDA_TOOLKIT_ROOT_DIR = ${CUDA_TOOLKIT_ROOT_DIR}")
message("CUDA_npp_LIBRARY = ${CUDA_npp_LIBRARY}")
message("CUDA_curand_LIBRARY = ${CUDA_curand_LIBRARY}")

if(CUDA_SDK_ROOT_DIR MATCHES "CUDA_SDK_ROOT_DIR-NOTFOUND")
	MESSAGE(FATAL_ERROR "Could not locate CUDA SDK. Try setting environment variable NVSDKCOMPUTE_ROOT or NVSDKCUDA_ROOT")
endif(CUDA_SDK_ROOT_DIR MATCHES "CUDA_SDK_ROOT_DIR-NOTFOUND")



INCLUDE(FindCUDA)
INCLUDE_DIRECTORIES ( ${CUDA_SDK_ROOT_DIR}/common/inc ${CUDA_SDK_ROOT_DIR}/../CUDALibraries/common/inc)

#LINK_LIBRARIES("${CUDA_SDK_ROOT_DIR}/lib -lcutil")
#set( CUDA_USE_LIBS cudart shrutil_x86_64 cutil_x86_64 )

# Build script starts here
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CUDA_NVCC_FLAGS -g;-G;-Xcompiler;-fmessage-length=0;--compiler-bindir /usr/bin/gcc-4.3)

# CUDA settings
#SET(CUDA_NVCC_FLAGS -DDEBUG -I/usr/local/cuda/include -I/home/id23cat/NVIDIA_GPU_Computing_SDK/C/common/inc/ -I/home/id23cat/NVIDIA_GPU_Computing_SDK/CUDALibraries/common/inc/ -O0 -g -G -c -Xcompiler -fmessage-length=0 --compiler-bindir /usr/bin/gcc-4.3 )
#SET(CUDA_NVCC_FLAGS_DEBUG "-g;-G")

#set( CUDA_USE_LIBS cudart shrutil_i386 cutil_i386 )
#find_package ( CUDA )

#include(${OpenCV_USE_FILE})

cuda_add_executable ( ${PROJECT} ${HEADERS} ${CUDA_SOURCES} ${SOURCES} OPTIONS )
target_link_libraries ( ${PROJECT} ${CUDA_LIBRARIES} )

