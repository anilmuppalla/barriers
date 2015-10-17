#include "MPI_combined_barrier.h"
#include <stdio.h>

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
	
	OMP_dissemination_barrier(thread_sense, parity);

	if (thread_id == 0) {

		MPI_tournament_barrier();

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