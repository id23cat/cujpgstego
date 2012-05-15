set (PROJECT KZjpeg)
project ( ${PROJECT}  )
# Поддержка CUDA появилась в CMake 2.8
cmake_minimum_required(VERSION 2.8)

# Build script starts here
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# CUDA settings
#SET(CUDA_NVCC_FLAGS -DDEBUG -I/usr/local/cuda/include -I/home/id23cat/NVIDIA_GPU_Computing_SDK/C/common/inc/ -I/home/id23cat/NVIDIA_GPU_Computing_SDK/CUDALibraries/common/inc/ -O0 -g -G -c -Xcompiler -fmessage-length=0 --compiler-bindir /usr/bin/gcc-4.3 )
SET(CUDA_NVCC_FLAGS_DEBUG "-g;-G")

set( CUDA_USE_LIBS cudart shrutil_i386 cutil_i386 )
#find_package ( CUDA )
find_package ( CUDA QUIET REQUIRED )
#include(${OpenCV_USE_FILE})
set ( SOURCES cujpeg.cpp DCTdataIterator.h DCTdataIterator.cpp Exceptions.h huftree.h huftree.cpp JPEGfile.h JPEGfile.cpp KZanalizer.h KZanalizer.cpp StreamReader.h StreamReader.cpp )
set ( CUDA_SOURCES KZanalizerCUDA.cu )
#cuda_add_executable ( ${PROJECT} ${SOURCES} ${CUDA_SOURCE} )
cuda_add_executable ( ${PROJECT} ${SOURCES} ${CUDA_SOURCE} OPTIONS -O0 -g -G -c -Xcompiler -fmessage-length=0 --compiler-bindir /usr/bin/gcc-4.3)
#target_link_libraries ( ${PROJECT} ${CUDA_USE_LIBS} )
#link_internal ( ${PROJECT} osgPPU )
#target_link_libraries(${PROJECT} ${CUDA_USE_LIBS})