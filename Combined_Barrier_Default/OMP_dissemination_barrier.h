#ifndef __OMP_DISSEMINATION_BARRIER_H__
#define __OMP_DISSEMINATION_BARRIER_H__ 1

#include <omp.h>

void OMP_dissemination_barrier(int *sense, int *parity);
void OMP_dissemination_setup(int threads);
void OMP_dissemination_end();

#endif
