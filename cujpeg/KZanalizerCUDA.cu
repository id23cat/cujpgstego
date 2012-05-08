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
#include "Exceptions.h"


//#define CUDA_CALL(x) if ( x  != cudaSuccess ) { \
		fprintf (stderr, " Error at %s :%d \n " , __FILE__ , __LINE__ ) ;\
		exit(EXIT_FAILURE) ;}

//KZanalizerCUDA::KZanalizerCUDA(JPEG::DCTdataIterator begin, JPEG::DCTdataIterator end, UINT8 comp):
//KZanalizer(begin, end, comp){
////	colorComponent = comp;
////	// calculate elements count
////	INT16* ptr1 = begin.getCurBlock();
////	INT16* ptr2 = end.getCurBlock();
////
////	size_t count = (ptr2 - ptr1);
////	blockCount = count/64;
////
////	dctLen = blockCount * 8;
////	if(colorComponent != _ALL){
////		UINT8* decim = begin.getDecimation();
////		dctLen = dctLen * decim[colorComponent]/ (decim[_Y] + decim[_CB] + decim[_CR]);
////	}
////	SAFE_MALLOC_INT16(dctPtr, dctLen);
////
////
////	KZdataIterator kzit(dctPtr, dctLen, begin.getDecimation());
////	JPEG::DCTdataIterator it = begin;
////
//////	printf("KZ constructor\n");
////	while(it < end){
////		if(colorComponent == _ALL || colorComponent == it.color()){
////			for(int i=0; i<8; i++)
////				kzit[i] = it[KochZhaoZZ_order[i]];
//////			it.PrintBlock();
//////			printf("\n");
//////			kzit.PrintBlock();
//////			printf("\n");
////			kzit.mvToNextBlock();
////		}
////		it.mvToNextBlock();
////	}
//}

//__constant__ INT16 *dcts;

// CUDA SDK sum reduction:
// /home/id23cat/NVIDIA_GPU_Computing_SDK/C/src/reduction/doc/
//template <unsigned int blockSize>
//__device__ void warpReduce(volatile int *sdata, unsigned int tid) {
//	if (blockSize >= 64) sdata[tid] += sdata[tid + 32];
//	if (blockSize >= 32) sdata[tid] += sdata[tid + 16];
//	if (blockSize >= 16) sdata[tid] += sdata[tid + 8];
//	if (blockSize >= 8) sdata[tid] += sdata[tid + 4];
//	if (blockSize >= 4) sdata[tid] += sdata[tid + 2];
//	if (blockSize >= 2) sdata[tid] += sdata[tid + 1];
//}
//#ifdef __DEVICE_EMULATION__
//#define EMUSYNC __syncthreads()
//#else
//#define EMUSYNC
//#endif

// Utility class used to avoid linker errors with extern
// unsized shared memory arrays with templated type
//template<class T>
//struct SharedMemory
//{
//    __device__ inline operator       T*()
//    {
//        extern __shared__ int __smem[];
//        return (T*)__smem;
//    }
//
//    __device__ inline operator const T*() const
//    {
//        extern __shared__ int __smem[];
//        return (T*)__smem;
//    }
//};

// specialize for double to avoid unaligned memory
// access compile errors
//template<>
//struct SharedMemory<double>
//{
//    __device__ inline operator       double*()
//    {
//        extern __shared__ double __smem_d[];
//        return (double*)__smem_d;
//    }
//
//    __device__ inline operator const double*() const
//    {
//        extern __shared__ double __smem_d[];
//        return (double*)__smem_d;
//    }
//};

/*
    This version is completely unrolled.  It uses a template parameter to achieve
    optimal code for any (power of 2) number of threads.  This requires a switch
    statement in the host code to handle all the different thread block sizes at
    compile time.

    Note, this kernel needs a minimum of 64*sizeof(T) bytes of shared memory.
    In other words if blockSize <= 32, allocate 64*sizeof(T) bytes.
    If blockSize > 32, allocate blockSize*sizeof(T) bytes.
*/
//template <class T, unsigned int blockSize>
//__global__ void
//reduce5(T *g_idata, T *g_odata, unsigned int n)
//{
//    T *sdata = SharedMemory<T>();
//
//    // perform first level of reduction,
//    // reading from global memory, writing to shared memory
//    unsigned int tid = threadIdx.x;
//    unsigned int i = blockIdx.x*(blockSize*2) + threadIdx.x;
//
//    T mySum = (i < n) ? g_idata[i] : 0;
//    if (i + blockSize < n)
//        mySum += g_idata[i+blockSize];
//
//    sdata[tid] = mySum;
//    __syncthreads();
//
//    // do reduction in shared mem
//    if (blockSize >= 512) { if (tid < 256) { sdata[tid] = mySum = mySum + sdata[tid + 256]; } __syncthreads(); }
//    if (blockSize >= 256) { if (tid < 128) { sdata[tid] = mySum = mySum + sdata[tid + 128]; } __syncthreads(); }
//    if (blockSize >= 128) { if (tid <  64) { sdata[tid] = mySum = mySum + sdata[tid +  64]; } __syncthreads(); }
//
//#ifndef __DEVICE_EMULATION__
//    if (tid < 32)
//#endif
//    {
//        // now that we are using warp-synchronous programming (below)
//        // we need to declare our shared memory volatile so that the compiler
//        // doesn't reorder stores to it and induce incorrect behavior.
//        volatile T* smem = sdata;
//        if (blockSize >=  64) { smem[tid] = mySum = mySum + smem[tid + 32]; EMUSYNC; }
//        if (blockSize >=  32) { smem[tid] = mySum = mySum + smem[tid + 16]; EMUSYNC; }
//        if (blockSize >=  16) { smem[tid] = mySum = mySum + smem[tid +  8]; EMUSYNC; }
//        if (blockSize >=   8) { smem[tid] = mySum = mySum + smem[tid +  4]; EMUSYNC; }
//        if (blockSize >=   4) { smem[tid] = mySum = mySum + smem[tid +  2]; EMUSYNC; }
//        if (blockSize >=   2) { smem[tid] = mySum = mySum + smem[tid +  1]; EMUSYNC; }
//    }
//
//    // write result for this block to global mem
//    if (tid == 0) g_odata[blockIdx.x] = sdata[0];
//}

#define PLUS(a, b) a += b
#define PLUSP(a, b) a += b*b
__global__ void GStd(INT16 *dct, VALUETYPE *pStd, VALUETYPE *pSum){
//	__shared__ VALUETYPE shsum[8];
//	__shared__ VALUETYPE shsum2[8];
//
	// perform first level of reduction,
	// reading from global memory, writing to shared memory
	unsigned int tid = threadIdx.x;
	unsigned int i = blockIdx.x*(blockDim.x*2) + threadIdx.x;
//
//	INT16 val = dct[i];
//	VALUETYPE sum = val;
//	VALUETYPE sum2 = val;
//
//	val = dct[i+blockDim.x];
//	PLUS(sum, val);
//	PLUSP(sum2, val);
//
//	shsum[tid] = sum;
//	shsum2[tid] = sum2;
//	__syncthreads();
//
//	volatile VALUETYPE *smem = shsum;
//	volatile VALUETYPE *smem2 = shsum2;
//
//	PLUS(sum, smem[tid + 2]);
//	PLUSP(sum2, smem2[tid + 2]);
//
//	PLUS(sum, smem[tid + 1]);
//	PLUSP(sum2, smem2[tid + 1]);
	pSum[blockIdx.x] = 1;
	dct[threadIdx.x + blockDim.x*blockIdx.x] = threadIdx.x;
	if(tid == 0){
//		VALUETYPE mean = sum/8;
//		pSum[blockIdx.x] = sum;
//		pStd[blockIdx.x] = sqrtf(sum2/8 - mean*mean);
		pSum[blockIdx.x] = 1;
		pStd[blockIdx.x] = blockIdx.x;
	}
};

//__device__ VALUETYPE gProbability;

//__global__ void GMean(INT16 *dct, size_t dctlen, VALUETYPE *mean){
//
//};
//
//__global__ void GStd(INT16 *dct, size_t dctlen, VALUETYPE *std){
//
//};
//
//__global__ void GAnalize(INT16 *dct, size_t dctlen, VALUETYPE *probability){
//
//};
//

int KZanalizerCUDA::InitMem(){
	cutilSafeCall(
			cudaMalloc(&dDCTptr, dctLen * sizeof(INT16)));
	cutilSafeCall(
			cudaMemcpy(dDCTptr, dctPtr, dctLen * sizeof(INT16), cudaMemcpyHostToDevice));
	cutilSafeCall(
			cudaMalloc(&dMean, blockCount * sizeof(VALUETYPE)));
	cutilSafeCall(
			cudaMalloc(&dStd, blockCount * sizeof(VALUETYPE)));
	cutilSafeCall(
			cudaMalloc(&dSum, blockCount * sizeof(VALUETYPE)));
//	cutilSafeCall(
//			cudaMalloc(&gHist, dctLen * sizeof(VALUETYPE)));


//	SAFE_MALLOC(hStd, blockCount, VALUETYPE);
	return dctLen;
}

bool KZanalizerCUDA::Analize(int Pthreshold ){
	InitMem();
	VALUETYPE *hStd = (VALUETYPE*) malloc( blockCount * sizeof(VALUETYPE));
	if(!hStd)
		fprintf(stderr, "hStd\n");
	VALUETYPE *hSum = (VALUETYPE*) malloc( blockCount * sizeof(VALUETYPE));
	if(!hSum)
		fprintf(stderr, "hSum\n");

	INT16 *m = (INT16*)malloc(dctLen * sizeof(INT16));

	for(int i=0; i<dctLen; i++)
			printf("DCT[%d]=%f\n", i, dctPtr[i]);


	dim3 blockSize(8);	//4
	dim3 gridSize(blockCount);
//	dim3 gridSize(10);
	GStd<<<gridSize, blockCount>>>(dDCTptr, dStd, dSum);

	cutilSafeCall(
				cudaMemcpy(m, dDCTptr, dctLen * sizeof(INT16), cudaMemcpyDeviceToHost));
	cutilSafeCall(
			cudaMemcpy(hSum, dSum, blockCount * sizeof(VALUETYPE), cudaMemcpyDeviceToHost));
	cutilSafeCall(
			cudaMemcpy(hStd, dStd, blockCount * sizeof(VALUETYPE), cudaMemcpyDeviceToHost));

	for(int i=0; i<gridSize.x; i++)
		printf("Sum[%d]=%f, Std[%d]=%f\n", i, hSum[i], i, hStd[i]);
	for(int i=0; i<dctLen; i++)
			printf("M[%d]=%d\n", i, m[i]);

	printf("Bloks = %d", blockCount);
	free (hStd);
	free (hSum);
	return false;
}

KZanalizerCUDA::~KZanalizerCUDA(){
	cutilSafeCall(
			cudaFree(dDCTptr));
	cutilSafeCall(
			cudaFree(dMean));
	cutilSafeCall(
			cudaFree(dStd));
	cutilSafeCall(
			cudaFree(dSum));
//	SAFE_FREE(hSum);
//	SAFE_FREE(hStd);
}
