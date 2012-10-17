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

//__align__(128) INT16 *dDCTptr;			// pointer in device memory

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

#define WARP_SIZE 32
#define HALF_WARP 16
#define HALF_KZBLOCK 4

#define BANK_COUNT 16	// 32

#define PLUS(a, b) a += b
#define PLUS_SQ(a, b) a += b*b

/******************* GStd1 *********************/
__global__ void GStd(INT16 *dct, INT16 *psum=NULL, INT16 *psumsq=NULL,
		VALUETYPE *pStd=NULL, VALUETYPE *pSum=NULL){
	__shared__ INT16 shsum[4];
	__shared__ INT16 shsumsq[4];

	// perform first level of reduction,
	// reading from global memory, writing to shared memory
	unsigned int tidx = threadIdx.x;
	unsigned int idxG = blockIdx.x*(blockDim.x*2) + threadIdx.x;

	INT16 val = dct[idxG];
	INT16 sum = val;		// SUM
	INT16 sumsq = val;		// SUM OF SQUARES

	val = dct[idxG + blockDim.x];
	PLUS(sum, val);
	PLUS_SQ(sumsq, val);

	shsum[tidx] = sum;
	shsumsq[tidx] = sumsq;
	__syncthreads();

	volatile INT16 *smem = shsum;
	volatile INT16 *smemsq = shsumsq;

	if(tidx < 2){
		PLUS(smem[tidx], smem[tidx + 2]);
		PLUS_SQ(smemsq[tidx], smemsq[tidx + 2]);
	}

	if(tidx == 0){
		PLUS(smem[tidx], smem[tidx + 1]);
		PLUS_SQ(smemsq[tidx], smemsq[tidx + 1]);

		dct[idxG] = smem[tidx];


//		psum[blockIdx.x] = smem[0];
//		psumsq[blockIdx.x] = smemsq[tidx];
////		VALUETYPE mean = sum/8;
////		pSum[blockIdx.x] = sum;
////		pStd[blockIdx.x] = sqrtf(sum2/8 - mean*mean);
//		pSum[blockIdx.x] = 1;
//		pStd[blockIdx.x] = blockIdx.x;
	}
};


/******************* GStd2 *********************/
__global__ void GStd2(INT16 *dct/*, INT16 *psum=NULL, INT16 *psumsq=NULL,
		VALUETYPE *pStd=NULL, VALUETYPE *pSum=NULL*/){
	__shared__ INT16 shsum[512*2];
	__shared__ INT16 shsumsq[512*2];
//	__shared__ INT16* shsum;
//	__shared__ INT16* shsumsq;
////
////	The first thread in the block does the allocation
////	and then shares the pointer with all other threads
////	through shared memory, so that access can easily be coalesced.
//	if(threadIdx.x == 0){
//		shsum = (INT16*)malloc(blockDim.x * sizeof(INT16));
//		shsumsq = (INT16*)malloc(blockDim.x * sizeof(INT16));
//	}
//	__syncthreads();


	// perform first level of reduction,
	// reading from global memory, writing to shared memory
	unsigned int tidx = threadIdx.x;
	unsigned int idxG = blockIdx.x*(blockDim.x*2) + threadIdx.x;

	// if tidx
//	unsigned int BASEidx = tidx%WARP_SIZE >= HALF_WARP;
//	unsigned int shidx = BASEidx + sizeof(INT16) * tidx;

	// for 1.x arch
	unsigned int warpBlock = WARP_SIZE;			// 1.x elements count in the warp
//	unsigned int warpSize = 2*WARP_SIZE;		// 2.x
//	unsigned int BASEidx1x = tidx % WARP_SIZE >= BANK_COUNT;		// BANK_COUNT=16
//	unsigned int BASEidx2x = tidx % (2*WARP_SIZE) >= BANK_COUNT;	// BANK_COUNT=32

	unsigned int BASEidx1x = tidx % warpBlock >= BANK_COUNT;
	BASEidx1x += (unsigned int)tidx/warpBlock * warpBlock;

	// !!!
	unsigned int shidx = BASEidx1x + sizeof(INT16) * (tidx%1);

	INT16 val = dct[idxG];
	INT16 sum = val;		// SUM
	INT16 sumsq = val;		// SUM OF SQUARES

	val = dct[idxG + HALF_KZBLOCK];
	PLUS(sum, val);
	PLUS_SQ(sumsq, val);

	shsum[shidx] = sum;
	shsumsq[shidx] = sumsq;
	__syncthreads();

	volatile INT16 *smem = shsum;
	volatile INT16 *smemsq = shsumsq;

	//// !!?????
	if(tidx < 2){
		PLUS(smem[tidx], smem[tidx + 2]);
		PLUS_SQ(smemsq[tidx], smemsq[tidx + 2]);
	}

	if(tidx == 0){
		PLUS(smem[tidx], smem[tidx + 1]);
		PLUS_SQ(smemsq[tidx], smemsq[tidx + 1]);

		dct[idxG] = smem[tidx];


//		psum[blockIdx.x] = smem[0];
//		psumsq[blockIdx.x] = smemsq[tidx];
////		VALUETYPE mean = sum/8;
////		pSum[blockIdx.x] = sum;
////		pStd[blockIdx.x] = sqrtf(sum2/8 - mean*mean);
//		pSum[blockIdx.x] = 1;
//		pStd[blockIdx.x] = blockIdx.x;
	}
};


/******************* GStd3 *********************/
typedef struct{
	INT32 x;
	INT32 y;
	INT32 z;
	INT32 w;
} my_uint4;

typedef struct sh2{
	INT16 x;
	INT16 y;
	__device__ inline sh2(UINT32 i){x = i&0xFFFF0000; y = i&0x0000FFFF;};
//	__device__ inline UINT32 toint(){return (UINT32)x<<16+y;};

} my_short2;

typedef struct{
	INT16 x0;
	INT16 x1;
	INT16 y0;
	INT16 y1;
	INT16 z0;
	INT16 z1;
	INT16 w0;
	INT16 w1;
} my_int4;

#define ALIGN_UP(offset, alignment) \
(offset) = ((offset) + (alignment) – 1) & ~((alignment) – 1)

__global__ void GStd3(INT16 *dct){
	my_int4 *ptr = (my_int4*)dct;
	int tidx = threadIdx.x + blockDim.x * blockIdx.x;
	my_int4 ui = ptr[tidx];


	ui.x0 += ui.x1;
	ui.x0 += ui.y0;
	ui.x0 += ui.y1;
	ui.x0 += ui.z0;
	ui.x0 += ui.z1;
	ui.x0 += ui.w0;
	ui.x0 += ui.w1;
	__syncthreads();

	ptr[tidx] = ui;
}

#define FIRST(X) X >> 16
#define LAST(X) X & 0x0000FFFF

__global__ void GStd4(INT16 *dct){
//	uint4 *ptr = (uint4*)dct;

	int tidx = threadIdx.x + blockDim.x * blockIdx.x;
	uint4 ui4 = ((uint4*)dct)[tidx];

	INT16 v = 0;

	v += FIRST(ui4.x);
	v += LAST(ui4.x);

	v += FIRST(ui4.y);
	v += LAST(ui4.y);

	v += FIRST(ui4.z);
	v += LAST(ui4.z);

	v += FIRST(ui4.w);
	v += LAST(ui4.w);

	ui4.x = (UINT32)v;

//	__syncthreads();

	((uint4*)dct)[tidx] = ui4;
}

__global__ void GStd5(INT16 *dct){

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

__global__ void GStd5_2(INT16 *dct){

	__shared__ INT16 shmem[SHMEM];	//256*8 //4096// 8*512
	int tidx = threadIdx.x + blockDim.x * blockIdx.x;
	((uint4*)shmem)[threadIdx.x] = ((uint4*)dct)[tidx];

	int shidx = threadIdx.x << 3;		// threadIdx.x*8;
	INT16 sum = 0;
	INT32 sumsq = 0;

//#pragma unroll 4
//	for(int i=0; i<8; i++)
//		v += shmem[shidx+i];

	SumSum( shmem[shidx], sum, sumsq);
	SumSum( shmem[shidx+1], sum, sumsq);

	SumSum(shmem[shidx+2], sum, sumsq);
	SumSum(shmem[shidx+3], sum, sumsq);

	SumSum(shmem[shidx+4], sum, sumsq);
	SumSum(shmem[shidx+5], sum, sumsq);

	SumSum(shmem[shidx+6], sum, sumsq);
	SumSum(shmem[shidx+7], sum, sumsq);

	shmem[shidx+0] = sum;
	shmem[shidx+1] = sumsq;

	((uint4*)dct)[tidx] = ((uint4*)shmem)[threadIdx.x];
}

__global__ void GStd6(INT16 *dct){

	__shared__ INT16 shmem[SHMEM];	//256*8 //4096// 8*512
	int tidx = threadIdx.x + blockDim.x * blockIdx.x;
	((uint4*)shmem)[threadIdx.x] = ((uint4*)dct)[tidx];

//	int shidx = threadIdx.x*8;
	int shidx = threadIdx.x << 3;
	INT16 v = 0;
//	int i0 = threadIdx.x%16 >> 2 << 1;		// ((( thid % 16 ) / 4 ) * 2 )
	int i0 = ( threadIdx.x - threadIdx.x >> 4) >> 2 << 1;

//	int idx = 0;
//#pragma unroll
//	for(int i=0; i<8; i++){
////		idx = ( i0 + i ) % 8;
//		int k = i0 + i;
//		idx = k -( k >> 3);
//		v += shmem[ shidx + idx ];
//	}

	int idx = i0 - (i0 >> 3);
	v += shmem[shidx + idx];
	i0++; idx = i0 - (i0 >> 3);
	v += shmem[shidx+idx];

	i0++; idx = i0 - (i0 >> 3);
	v += shmem[shidx+idx];
	i0++; idx = i0 - (i0 >> 3);
	v += shmem[shidx+idx];

	i0++; idx = i0 - (i0 >> 3);
	v += shmem[shidx+idx];
	i0++; idx = i0 - (i0 >> 3);
	v += shmem[shidx+idx];

	i0++; idx = i0 - (i0 >> 3);
	v += shmem[shidx+idx];
	i0++; idx = i0 - (i0 >> 3);
	v += shmem[shidx+idx];

	shmem[shidx] = v;

	((uint4*)dct)[tidx] = ((uint4*)shmem)[threadIdx.x];
}

//typedef MEM<INT16> HOST_I16;
//typedef MEM<INT16> DEV_I16;
//typedef MEM<VALUETYPE> HOST_F32;
//typedef MEM<VALUETYPE> DEV_F32;

//#define MEM_H2D(H, D, TYPE) cutilSafeCall(cudaMalloc(&D.ptr, dctLen * sizeof(INT16)));

//inline cudaError_t HostToDev(DEV_I16 dst, HOST_I16 src = MEM<INT16>(0)){
//	if(dst.ptr == NULL){
//		if(dst.length > src.length)
//			cutilSafeCall(
//				cudaMalloc(&dst.ptr, dst.length * sizeof(INT16)));
//		else if(src.length){
//			cutilSafeCall(
//				cudaMalloc(&dst.ptr, src.length * sizeof(INT16)));
//			dst.length = src.length;
//		}
//		else return cudaErrorMemoryAllocation;
//	}else if(dst.length < src.length){
//		cudaFree(dst.ptr);
//		cutilSafeCall(
//			cudaMalloc(&dst.ptr, src.length * sizeof(INT16)));
//		dst.length = src.length;
//	}
//	if(src.ptr != NULL && src.length)
//		cutilSafeCall(
//			cudaMemcpy(dst.ptr, src.ptr, dst.length * sizeof(INT16), cudaMemcpyHostToDevice));
//
//	return cudaSuccess;
//}

int KZanalizerCUDA::InitMem(){
	cutilSafeCall(
			cudaMalloc(&dDCTptr, dctLen * sizeof(INT16)));
	cutilSafeCall(
			cudaMemcpy(dDCTptr, dctPtr, dctLen * sizeof(INT16), cudaMemcpyHostToDevice));
//	cutilSafeCall(
//			cudaMalloc(&dMean, blockCount * sizeof(VALUETYPE)));
//	cutilSafeCall(
//			cudaMalloc(&dStd, blockCount * sizeof(VALUETYPE)));
//	cutilSafeCall(
//			cudaMalloc(&dSum, blockCount * sizeof(VALUETYPE)));
//	cutilSafeCall(
//			cudaMalloc(&gHist, dctLen * sizeof(VALUETYPE)));


//	SAFE_MALLOC(hStd, blockCount, VALUETYPE);
	return dctLen;
}

bool KZanalizerCUDA::Analize(int Pthreshold ){

	InitMem();

//	int shMpT = 2*sizeof(INT16);	// shared memory per thread in bytes;
//	int thcount = ColcMaxThreadsPerBLock(shMpT, 8, dctLen * sizeof(INT16), 4);
//	int blkcount = CalcBlockCount(shMpT, dctLen * sizeof(INT16), thcount);
//	printf("Threads count = %d, blocks count = %d (totMem = %d)\n",
//			thcount, blkcount, dctLen * sizeof(INT16));


//	dim3 blockSize(4);	//4
//	dim3 gridSize(blockCount);
////	dim3 gridSize(10);
//	GStd<<<gridSize, blockCount>>>(dDCTptr, dStd, dSum);
//	GStd<<<gridSize, blockSize>>>(dDCTptr, dsum, dsumsq);


//	GStd<<<gridSize, blockSize>>>( dDCTptr );

	int threads = THREADS;
	printf("Threads count = %d, blocks count = %d\n", threads, blockCount/threads+1);
#ifdef TIME_COMPARE
	Timer timer;
	timer.Start();
#endif
	int iMAX=1000;
	for(int i=0; i<iMAX; i++){
//	GStd3<<<blockCount/threads+1, threads>>>( dDCTptr );
//	GStd3<<<4, 4>>>( dDCTptr );
//	GStd4<<<blockCount/threads+1, threads>>>( dDCTptr );
//	GStd5<<<blockCount/threads+1, threads>>>( dDCTptr );

	GStd5_2<<<blockCount/threads+1, threads>>>( dDCTptr );
//	GStd6<<<blockCount/threads+1, threads>>>( dDCTptr );
	}
#ifdef TIME_COMPARE
	float time1 = timer.Stop()/iMAX;
	printf("GPU STD5_2: %.5fs\n", time1);
#endif

#ifdef TIME_COMPARE
	timer.Start();
#endif
	for(int i=0; i<iMAX; i++){
		GStd6<<<blockCount/threads+1, threads>>>( dDCTptr );
	}
#ifdef TIME_COMPARE
	time1 = timer.Stop()/iMAX;
	printf("GPU STD6: %.5fs\n", time1);
#endif

#ifdef TIME_COMPARE
	timer.Start();
#endif
	for(int i=0; i<iMAX; i++){
		GStd<<<blockCount/threads+1, threads>>>( dDCTptr );
	}
#ifdef TIME_COMPARE
	time1 = timer.Stop()/iMAX;
	printf("GPU STD: %.5fs\n", time1);
#endif

#ifdef TIME_COMPARE
	timer.Start();
#endif
	for(int i=0; i<iMAX; i++){
		GStd2<<<blockCount/threads+1, threads>>>( dDCTptr );
	}
#ifdef TIME_COMPARE
	time1 = timer.Stop()/iMAX;
	printf("GPU STD2: %.5fs\n", time1);
#endif

#ifdef TIME_COMPARE
	timer.Start();
#endif
	for(int i=0; i<iMAX; i++){
		GStd3<<<blockCount/threads+1, threads>>>( dDCTptr );
	}
#ifdef TIME_COMPARE
	time1 = timer.Stop()/iMAX;
	printf("GPU STD3: %.5fs\n", time1);
#endif

#ifdef TIME_COMPARE
	timer.Start();
#endif
	for(int i=0; i<iMAX; i++){
		GStd4<<<blockCount/threads+1, threads>>>( dDCTptr );
	}
#ifdef TIME_COMPARE
	time1 = timer.Stop()/iMAX;
	printf("GPU STD4: %.5fs\n", time1);
#endif

#ifdef TIME_COMPARE
	timer.Start();
#endif
	for(int i=0; i<iMAX; i++){
		GStd5<<<blockCount/threads+1, threads>>>( dDCTptr );
	}
#ifdef TIME_COMPARE
	time1 = timer.Stop()/iMAX;
	printf("GPU STD5: %.5fs\n", time1);
#endif

	INT16 *ppp;
	SAFE_HOST_MALLOC(ppp, dctLen, INT16);
//	COPY_TO_HOST(hsum, dsum, blockCount, INT16);
//	COPY_TO_HOST(hsumsq, dsumsq, blockCount, INT16);
	COPY_TO_HOST(ppp, dDCTptr, dctLen, INT16);

//	for(int i=0,k=0,j=0; i<dctLen; i++){
//		printf("DCT[%d]=%d DCT[%d]=%d\n", i, dctPtr[i], i, ppp[i]);
//		k++;
//		if( k== 8){
////			printf("\t SUM[%d]=%d, SUMSQ[%d]=%d\n", j, hsum[j], j, hsumsq[j]);
//			printf("\t[%d]=%d\n", j, ppp[i-7]);
//			j++;
//			k=0;
//		}
//	}


//
//	cutilSafeCall(
//				cudaMemcpy(m, dDCTptr, dctLen * sizeof(INT16), cudaMemcpyDeviceToHost));
//	cutilSafeCall(
//			cudaMemcpy(hSum, dSum, blockCount * sizeof(VALUETYPE), cudaMemcpyDeviceToHost));
//	cutilSafeCall(
//			cudaMemcpy(hStd, dStd, blockCount * sizeof(VALUETYPE), cudaMemcpyDeviceToHost));
//
//	for(int i=0; i<gridSize.x; i++)
//		printf("Sum[%d]=%f, Std[%d]=%f\n", i, hSum[i], i, hStd[i]);
//	for(int i=0; i<dctLen; i++)
//			printf("M[%d]=%d\n", i, m[i]);
//
//	printf("Bloks = %d", blockCount);
//	free (hStd);
//	free (hSum);
	return false;
}

KZanalizerCUDA::KZanalizerCUDA(JPEG::DCTdataIterator begin, JPEG::DCTdataIterator end, UINT8 component):
KZanalizer(begin, end, component){
	cudaDeviceReset();
};

KZanalizerCUDA::~KZanalizerCUDA(){
	SAFE_DEVICE_FREE(dDCTptr);
//	cutilSafeCall(
//			cudaFree(dMean));
//	cutilSafeCall(
//			cudaFree(dStd));
//	cutilSafeCall(
//			cudaFree(dSum));
//	SAFE_HOST_FREE(hSum);
//	SAFE_HOST_FREE(hStd);
}
