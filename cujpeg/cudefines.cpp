/*
 * cudefines.cpp
 *
 *  Created on: 25.05.2012
 *      Author: id23cat
 */
#include "cudefines.h"
#include <cuda.h>
#include <math.h>


cudaEvent_t start, stop;
float elapsedTime;

//// This function wraps the CUDA Driver API into a template function
//template <class T>
//inline void getCudaAttribute(T *attribute, CUdevice_attribute device_attribute, int device)
//{
//    CUresult error = 	cuDeviceGetAttribute( attribute, device_attribute, device );
//
//    if( CUDA_SUCCESS != error) {
//        fprintf(stderr, "cuSafeCallNoSync() Driver API error = %04d from file <%s>, line %i.\n",
//                error, __FILE__, __LINE__);
//        exit(-1);
//    }
//}

int ColcMaxThreadsPerBLock(int shMemPerThread, int usedRegisters,
		int totalNeededSharedMem, int multNUM){
	int dev;
	cutilSafeCall(cudaGetDevice(&dev));

	cudaDeviceProp devProp;
	cutilSafeCall(cudaGetDeviceProperties(&devProp, dev));

	int thcount = devProp.maxThreadsPerBlock;	// threads count

	int shMem = (devProp.sharedMemPerBlock-128)/shMemPerThread;
	if(shMem < thcount)	thcount = shMem;

	int regs = devProp.regsPerBlock/usedRegisters;
	if(regs < thcount)	thcount = regs;

	int totMem = totalNeededSharedMem/shMemPerThread;
	if(totMem < thcount)	thcount = totMem;

	if(multNUM){
//		printf("%d\n", thcount%multNUM);
		printf("%d\n", 48%32>=16);
		thcount -= thcount%multNUM;
	}

	return thcount;
}

int CalcBlockCount(int shMemPerThread, int totalNeededSharedMem, int threadCount){
//	printf("%f\n", (double)totalNeededSharedMem/(threadCount * shMemPerThread));
	int blccount = (int)ceil((double)totalNeededSharedMem/(threadCount * shMemPerThread));
	return blccount;
}
