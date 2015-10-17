#ifndef __MPI_SENSE_REVERSAL_BARRIER_H__
#define __MPI_SENSE_REVERSAL_BARRIER_H__ 1

#include "mpi.h"

void MPI_sense_reversal_setup(MPI_Comm comm);
void MPI_sense_reversal_barrier();
void MPI_sense_reversal_end();

#endif //__MPI_SENSE_REVERSAL_BARRIER_H__
