/*
 * Timer.h
 *
 *  Created on: 17.10.2012
 *      Author: id23cat
 */

#ifndef TIMER_H_
#define TIMER_H_
#include <cuda_runtime.h>
/*
 *
 */
class Timer {
	cudaEvent_t start, stop;
	float elapsedTime;			// time counter
public:
	Timer();
	virtual ~Timer();
	void Start();
	void Stop();
	float Stop(char *str);	// returns total time
	float DevideBy(int value);	// devide result by value
	void Print(char *str);
};

#endif /* TIMER_H_ */
