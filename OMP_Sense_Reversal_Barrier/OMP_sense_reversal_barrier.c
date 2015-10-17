#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int NUM_THREADS;
int count;

void OMP_sense_reversal_setup(int threads)
{
	NUM_THREADS = threads;
	count = threads;
	return;
}

void OMP_sense_reversal_barrier(int *local_sense, int *sense)
{
	int local_count;
	*local_sense = !(*local_sense);

	#pragma omp atomic capture
	local_count = --count;

	if (local_count == 0) {
		count = NUM_THREADS;
		*sense = *local_sense;
	}
	else
		while ( *sense != *local_sense );
		
}

void OMP_sense_reversal_end()
{
	return;
}