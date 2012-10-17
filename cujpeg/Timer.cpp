/*
 * Timer.cpp
 *
 *  Created on: 17.10.2012
 *      Author: id23cat
 */

#include "Timer.h"
#include <stdio.h>

Timer::Timer() {
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
}

Timer::~Timer() {
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
}

void Timer::Start(){

	cudaEventRecord(start, 0);
}

void Timer::Stop(char *str){
	Stop();
	Print(str);
}

void Timer::Stop(){
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);
}

void Timer::Print(char *str){
	if(str) printf("%s time: %.5fms\n", str, elapsedTime);
	else	printf("Elapsed time: %.5fms\n", elapsedTime);
}
