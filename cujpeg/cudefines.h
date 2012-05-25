/*
 * cudefines.hpp
 *
 *  Created on: 25.05.2012
 *      Author: id23cat
 */

#ifndef CUDEFINES_HPP_
#define CUDEFINES_HPP_

#include <cuda_runtime.h>
#include <stdio.h>

extern cudaEvent_t start, stop;
extern float elapsedTime;			// time counter

#define TIMER_START()\
	cudaEventCreate(&start);\
	cudaEventCreate(&stop); \
	cudaEventRecord(start, 0);

#define TIMER_STOP(str)\
		cudaEventRecord(stop, 0);\
		cudaEventSynchronize(stop);\
		cudaEventElapsedTime(&elapsedTime, start, stop);\
		cudaEventDestroy(start); cudaEventDestroy(stop);\
		if(str) printf("%s time: %.5fms\n", str, elapsedTime);

#endif /* CUDEFINES_HPP_ */
