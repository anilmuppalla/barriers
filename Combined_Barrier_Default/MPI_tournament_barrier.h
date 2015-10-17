#ifndef __MPI_TOURNAMENT_BARRIER_H__
#define __MPI_TOURNAMENT_BARRIER_H__ 1

#include "mpi.h"

void MPI_tournament_setup(MPI_Comm comm);
void MPI_tournament_barrier();
void MPI_tournament_end();

#endif //__MPI_TOURNAMENT_BARRIER_H__