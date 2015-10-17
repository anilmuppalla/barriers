#ifndef __MPI_COMNINED_BARRIER_H__
#define __MPI_COMNINED_BARRIER_H__ 1

#include "MPI_tournament_barrier.h"
#include "OMP_dissemination_barrier.h"

void MPI_combined_barrier_setup(MPI_Comm comm, int threads);
void MPI_combined_barrier();
void MPI_combined_barrier_end();

#endif //__MPI_COMNINED_BARRIER_H__