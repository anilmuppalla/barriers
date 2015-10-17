#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define USLEEP

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
#ifdef USLEEP
	useconds_t sec;
	sec = 2;
#endif

	#pragma omp atomic capture
	local_count = --count;

	if (local_count == 0) {
		count = NUM_THREADS;
		*sense = *local_sense;
	}
	else {
#ifndef USLEEP
		while ( *sense != *local_sense );
#else
		while ( *sense != *local_sense ) {

			usleep(sec);
			if (sec <= 256)
				sec *= 2;

		}
#endif
	}
			
		
}

void OMP_sense_reversal_end()
{
	return;
}