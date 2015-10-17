#ifndef __OMP_SENSE_REVERSAL_BARRIER_H__
#define __OMP_SENSE_REVERSAL_BARRIER_H__ 1

#include <omp.h>

void OMP_sense_reversal_barrier(int *local_sense, int *sense);
void OMP_sense_reversal_setup(int threads);
void OMP_sense_reversal_end();


#endif	//__OMP_SENSE_REVERSAL_BARRIER_H__