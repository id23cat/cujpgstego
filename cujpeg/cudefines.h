/*
 * cudefines.hpp
 *
 *  Created on: 25.05.2012
 *      Author: id23cat
 */

#ifndef CUDEFINES_HPP_
#define CUDEFINES_HPP_

#include <cutil_inline.h>
#include <cuda_runtime.h>
#include <cuda_runtime.h>
#include <stdio.h>

extern cudaEvent_t start, stop;
extern float elapsedTime;			// time counter

#define TIMER_START()\
	cudaEventCreate(&start);\
	cudaEventCreate(&stop); \
	cudaEventRecord(start, 0);

#define TIMER_STOP( str )\
		cudaEventRecord(stop, 0);\
		cudaEventSynchronize(stop);\
		cudaEventElapsedTime(&elapsedTime, start, stop);\
		cudaEventDestroy(start); cudaEventDestroy(stop);\
		if(str) printf("%s time: %.5fms\n", str, elapsedTime);


#define SAFE_HOST_MALLOC( ptr, count, TYPE )\
		if(!(ptr = (TYPE*) malloc(count * sizeof(TYPE)))){\
		fprintf(stderr, "%s(%d): malloc (%d) bytes -- failed",\
				__FILE__, __LINE__, count * sizeof(TYPE));\
		exit(EXIT_FAILURE);}
#define SAFE_HOST_FREE( ptr )\
		if(ptr) free(ptr);

#define SAFE_DEVICE_MALLOC( ptr, count, TYPE )\
		cutilSafeCall(cudaMalloc(&ptr, count * sizeof(TYPE)));

#define SAFE_DEVICE_FREE( ptr )\
		cutilSafeCall(cudaFree(ptr));

#define COPY_TO_DEVICE( dst, src, count, TYPE )\
		cutilSafeCall(cudaMemcpy(dst, src, count * sizeof(TYPE), cudaMemcpyHostToDevice));

#define COPY_TO_HOST( dst, src, count, TYPE )\
		cutilSafeCall(cudaMemcpy(dst, src, count * sizeof(TYPE), cudaMemcpyDeviceToHost));

// This function calculates optimal available count of threads per block
// neede to manipulate witn data
// shMemPerThread -- count of shatred memory needed per thread, in bytes
// usedRegisters -- count of registers needed per thread
// totalNeededSharedMem -- total memory size to need to share for threads, in bytes
// multNUM -- count must be multiple of multNUM
int ColcMaxThreadsPerBLock(int shMemPerThread, int usedRegisters,
		int totalNeededSharedMem, int multNUM=0);

// this function calculates block count
// shMemPerThread -- count of shatred memory needed per thread, in bytes
// totalNeededSharedMem -- total memory size to need to share for threads, in bytes
// threadCount -- count of threads
int CalcBlockCount(int shMemPerThread, int totalNeededSharedMem, int threadCount);

#endif /* CUDEFINES_HPP_ */
