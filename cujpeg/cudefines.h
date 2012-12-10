/*
 * cudefines.h
 *
 *  Created on: 25.05.2012
 *      Author: id23cat
 */

#ifndef CUDEFINES_H_
#define CUDEFINES_H_

//#include <cutil_inline.h>
#include <cuda_runtime.h>
#include <stdio.h>

//extern cudaEvent_t start, stop;
//extern float elapsedTime;			// time counter
//
//#define TIMER_START()\
//	cudaEventCreate(&start);\
//	cudaEventCreate(&stop); \
//	cudaEventRecord(start, 0);
//
//#define TIMER_STOP( str )\
//		cudaEventRecord(stop, 0);\
//		cudaEventSynchronize(stop);\
//		cudaEventElapsedTime(&elapsedTime, start, stop);\
//		cudaEventDestroy(start); cudaEventDestroy(stop);\
//		if(str) printf("%s time: %.5fms\n", str, elapsedTime);



static const char *_cudaGetErrorEnum(cudaError_t error)
{
    switch (error)
    {
        case cudaSuccess:
            return "cudaSuccess";

        case cudaErrorMissingConfiguration:
            return "cudaErrorMissingConfiguration";

        case cudaErrorMemoryAllocation:
            return "cudaErrorMemoryAllocation";

        case cudaErrorInitializationError:
            return "cudaErrorInitializationError";

        case cudaErrorLaunchFailure:
            return "cudaErrorLaunchFailure";

        case cudaErrorPriorLaunchFailure:
            return "cudaErrorPriorLaunchFailure";

        case cudaErrorLaunchTimeout:
            return "cudaErrorLaunchTimeout";

        case cudaErrorLaunchOutOfResources:
            return "cudaErrorLaunchOutOfResources";

        case cudaErrorInvalidDeviceFunction:
            return "cudaErrorInvalidDeviceFunction";

        case cudaErrorInvalidConfiguration:
            return "cudaErrorInvalidConfiguration";

        case cudaErrorInvalidDevice:
            return "cudaErrorInvalidDevice";

        case cudaErrorInvalidValue:
            return "cudaErrorInvalidValue";

        case cudaErrorInvalidPitchValue:
            return "cudaErrorInvalidPitchValue";

        case cudaErrorInvalidSymbol:
            return "cudaErrorInvalidSymbol";

        case cudaErrorMapBufferObjectFailed:
            return "cudaErrorMapBufferObjectFailed";

        case cudaErrorUnmapBufferObjectFailed:
            return "cudaErrorUnmapBufferObjectFailed";

        case cudaErrorInvalidHostPointer:
            return "cudaErrorInvalidHostPointer";

        case cudaErrorInvalidDevicePointer:
            return "cudaErrorInvalidDevicePointer";

        case cudaErrorInvalidTexture:
            return "cudaErrorInvalidTexture";

        case cudaErrorInvalidTextureBinding:
            return "cudaErrorInvalidTextureBinding";

        case cudaErrorInvalidChannelDescriptor:
            return "cudaErrorInvalidChannelDescriptor";

        case cudaErrorInvalidMemcpyDirection:
            return "cudaErrorInvalidMemcpyDirection";

        case cudaErrorAddressOfConstant:
            return "cudaErrorAddressOfConstant";

        case cudaErrorTextureFetchFailed:
            return "cudaErrorTextureFetchFailed";

        case cudaErrorTextureNotBound:
            return "cudaErrorTextureNotBound";

        case cudaErrorSynchronizationError:
            return "cudaErrorSynchronizationError";

        case cudaErrorInvalidFilterSetting:
            return "cudaErrorInvalidFilterSetting";

        case cudaErrorInvalidNormSetting:
            return "cudaErrorInvalidNormSetting";

        case cudaErrorMixedDeviceExecution:
            return "cudaErrorMixedDeviceExecution";

        case cudaErrorCudartUnloading:
            return "cudaErrorCudartUnloading";

        case cudaErrorUnknown:
            return "cudaErrorUnknown";

        case cudaErrorNotYetImplemented:
            return "cudaErrorNotYetImplemented";

        case cudaErrorMemoryValueTooLarge:
            return "cudaErrorMemoryValueTooLarge";

        case cudaErrorInvalidResourceHandle:
            return "cudaErrorInvalidResourceHandle";

        case cudaErrorNotReady:
            return "cudaErrorNotReady";

        case cudaErrorInsufficientDriver:
            return "cudaErrorInsufficientDriver";

        case cudaErrorSetOnActiveProcess:
            return "cudaErrorSetOnActiveProcess";

        case cudaErrorInvalidSurface:
            return "cudaErrorInvalidSurface";

        case cudaErrorNoDevice:
            return "cudaErrorNoDevice";

        case cudaErrorECCUncorrectable:
            return "cudaErrorECCUncorrectable";

        case cudaErrorSharedObjectSymbolNotFound:
            return "cudaErrorSharedObjectSymbolNotFound";

        case cudaErrorSharedObjectInitFailed:
            return "cudaErrorSharedObjectInitFailed";

        case cudaErrorUnsupportedLimit:
            return "cudaErrorUnsupportedLimit";

        case cudaErrorDuplicateVariableName:
            return "cudaErrorDuplicateVariableName";

        case cudaErrorDuplicateTextureName:
            return "cudaErrorDuplicateTextureName";

        case cudaErrorDuplicateSurfaceName:
            return "cudaErrorDuplicateSurfaceName";

        case cudaErrorDevicesUnavailable:
            return "cudaErrorDevicesUnavailable";

        case cudaErrorInvalidKernelImage:
            return "cudaErrorInvalidKernelImage";

        case cudaErrorNoKernelImageForDevice:
            return "cudaErrorNoKernelImageForDevice";

        case cudaErrorIncompatibleDriverContext:
            return "cudaErrorIncompatibleDriverContext";

        case cudaErrorPeerAccessAlreadyEnabled:
            return "cudaErrorPeerAccessAlreadyEnabled";

        case cudaErrorPeerAccessNotEnabled:
            return "cudaErrorPeerAccessNotEnabled";

        case cudaErrorDeviceAlreadyInUse:
            return "cudaErrorDeviceAlreadyInUse";

        case cudaErrorProfilerDisabled:
            return "cudaErrorProfilerDisabled";

        case cudaErrorProfilerNotInitialized:
            return "cudaErrorProfilerNotInitialized";

        case cudaErrorProfilerAlreadyStarted:
            return "cudaErrorProfilerAlreadyStarted";

        case cudaErrorProfilerAlreadyStopped:
            return "cudaErrorProfilerAlreadyStopped";

#if __CUDA_API_VERSION >= 0x4000

        case cudaErrorAssert:
            return "cudaErrorAssert";

        case cudaErrorTooManyPeers:
            return "cudaErrorTooManyPeers";

        case cudaErrorHostMemoryAlreadyRegistered:
            return "cudaErrorHostMemoryAlreadyRegistered";

        case cudaErrorHostMemoryNotRegistered:
            return "cudaErrorHostMemoryNotRegistered";
#endif

        case cudaErrorStartupFailure:
            return "cudaErrorStartupFailure";

        case cudaErrorApiFailureBase:
            return "cudaErrorApiFailureBase";
    }

    return "<unknown>";
}

template< typename T >
bool check(T result, char const *const func, const char *const file, int const line)
{
    if (result)
    {
        fprintf(stderr, "CUDA error at %s:%d code=%d(%s) \"%s\" \n",
                file, line, static_cast<unsigned int>(result), _cudaGetErrorEnum(result), func);
        /*
                std::stringstream ss;
                std::string msg("CUDA error at ");
                msg += file;
                msg += ":";
                ss << line;
                msg += ss.str();
                msg += " code=";
                ss << static_cast<unsigned int>(result);
                msg += ss.str();
                msg += " (";
                msg += _cudaGetErrorEnum(result);
                msg += ") \"";
                msg += func;
                msg += "\"";
                //throw msg;
                std::cerr  << msg <<"\n";
        */
        return true;
    }
    else
    {
        return false;
    }
}

#define SAFE_CALL(val) check ( (val), #val, __FILE__, __LINE__ )

#define SAFE_HOST_MALLOC( ptr, count, TYPE )\
		if(!(ptr = (TYPE*) malloc(count * sizeof(TYPE)))){\
		fprintf(stderr, "%s(%d): malloc (%d) bytes -- failed",\
				__FILE__, __LINE__, count * sizeof(TYPE));\
		exit(EXIT_FAILURE);}
#define SAFE_HOST_FREE( ptr )\
		if(ptr) free(ptr);

#define SAFE_DEVICE_MALLOC( ptr, count, TYPE )\
		SAFE_CALL(cudaMalloc(&ptr, count * sizeof(TYPE)));

#define SAFE_DEVICE_FREE( ptr )\
		SAFE_CALL(cudaFree(ptr));

#define COPY_TO_DEVICE( dst, src, count, TYPE )\
		SAFE_CALL(cudaMemcpy(dst, src, count * sizeof(TYPE), cudaMemcpyHostToDevice));

#define COPY_TO_HOST( dst, src, count, TYPE )\
		SAFE_CALL(cudaMemcpy(dst, src, count * sizeof(TYPE), cudaMemcpyDeviceToHost));

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

#endif /* CUDEFINES_H_ */
