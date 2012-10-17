/*
 * KZanalizerCUDA.cu
 *
 *  Created on: 02.05.2012
 *      Author: id23cat
 */

//#include "KZanalizerCUDA.cuh"
#include <cutil_inline.h>
#include <cuda_runtime.h>
#include "KZanalizer.h"
#include "datatypes.h"
#include "cudefines.h"
#ifdef TIME_COMPARE
#include "Timer.h"
#endif

#if (__CUDA_ARCH__ < 200)
#define THREADS 256
#else
#define THREADS 512
#endif
#define SHMEM THREADS*8



#define WARP_SIZE 32
#define HALF_WARP 16
#define HALF_KZBLOCK 4

#define BANK_COUNT 16	// 32

#define PLUS(a, b) a += b
#define PLUS_SQ(a, b) a += b*b



__global__ void GStd(INT16 *dct){

	__shared__ INT16 shmem[SHMEM];	//256*8 //4096// 8*512
	int tidx = threadIdx.x + blockDim.x * blockIdx.x;
	((uint4*)shmem)[threadIdx.x] = ((uint4*)dct)[tidx];

	int shidx = threadIdx.x*8;
	INT16 v = 0;

//#pragma unroll 4
//	for(int i=0; i<8; i++)
//		v += shmem[shidx+i];
	v += shmem[shidx];
	v += shmem[shidx+1];

	v += shmem[shidx+2];
	v += shmem[shidx+3];

	v += shmem[shidx+4];
	v += shmem[shidx+5];

	v += shmem[shidx+6];
	v += shmem[shidx+7];

	shmem[shidx+0] = v;

	((uint4*)dct)[tidx] = ((uint4*)shmem)[threadIdx.x];
}

__device__ inline void SumSum(INT16 val, INT16 &sum, INT32 &sumsq){
	sum +=val;
	sumsq += val*val;
}



int KZanalizerCUDA::InitMem(){
	cutilSafeCall(
			cudaMalloc(&dDCTptr, dctLen * sizeof(INT16)));
	cutilSafeCall(
			cudaMemcpy(dDCTptr, dctPtr, dctLen * sizeof(INT16), cudaMemcpyHostToDevice));

	return dctLen;
}

bool KZanalizerCUDA::Analize(int Pthreshold ){

	InitMem();

	int threads = THREADS;
	printf("Threads count = %d, blocks count = %d\n", threads, blockCount/threads+1);

#ifdef TIME_COMPARE
	Timer timer;
	timer.Start();
#endif

		GStd<<<blockCount/threads+1, threads>>>( dDCTptr );

#ifdef TIME_COMPARE
	float time = timer.Stop();
	printf("GPU STD5: %.5fs\n", time);
#endif

	INT16 *ppp;
	SAFE_HOST_MALLOC(ppp, dctLen, INT16);

	COPY_TO_HOST(ppp, dDCTptr, dctLen, INT16);

	return false;
}

KZanalizerCUDA::KZanalizerCUDA(JPEG::DCTdataIterator begin, JPEG::DCTdataIterator end, UINT8 component):
KZanalizer(begin, end, component){
	cudaDeviceReset();
};

KZanalizerCUDA::~KZanalizerCUDA(){
	SAFE_DEVICE_FREE(dDCTptr);
}
