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

float Timer::Stop(char *str){
	Stop();
	Print(str);
	return time;
}

float Timer::Stop(){
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&time, start, stop);
	return time;
}

float Timer::DevideBy(int value){
	return time /= value;
}

void Timer::Print(char *str){
	if(str) printf("%s time: %.5fms\n", str, time);
	else	printf("Elapsed time: %.5fms\n", time);
}
