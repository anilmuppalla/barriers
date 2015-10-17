#include "MPI_combined_barrier.h"
#include <stdio.h>

#define DEFAULT_BARRIER

int sense;

void MPI_combined_barrier_setup(MPI_Comm comm, int threads)
{
	sense = 0;

	MPI_tournament_setup(comm);
	OMP_dissemination_setup(threads);

	return;
}

void MPI_combined_barrier(int *thread_sense, int *parity, int *sense)
{
	int thread_id = omp_get_thread_num();
	int local_sense = *sense;

#ifndef DEFAULT_BARRIER	
	OMP_dissemination_barrier(thread_sense, parity);
#else
	{
		#pragma omp barrier
	}
#endif

	if (thread_id == 0) {
#ifndef DEFAULT_BARRIER
		MPI_tournament_barrier();
#else
		MPI_Barrier(MPI_COMM_WORLD);
#endif
		#pragma omp critical
		{
			*sense = !(*sense);
		}
	}
	else
		while(local_sense == *sense);

	return;
}

void MPI_combined_barrier_end()
{

	MPI_tournament_end();

	OMP_dissemination_end();

	return;
}