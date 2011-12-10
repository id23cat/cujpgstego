/*
 * cudaComputer.cuh
 *
 *  Created on: 06.12.2011
 *      Author: id23cat
 */

#ifndef CUDACOMPUTER_CUH_
#define CUDACOMPUTER_CUH_

/*
 *
 */
class cudaComputer {
	float *devPtr;
	float *hostPtr;
protected:
	virtual void memCopyHtoD()=0;

public:
	cudaComputer();
	virtual ~cudaComputer();
};

#endif /* CUDACOMPUTER_CUH_ */
