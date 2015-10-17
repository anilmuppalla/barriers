#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "OMP_dissemination_barrier.h"
#include "omp.h"

int NUM_THREADS, NUM_BARRIERS;

typedef struct flags {
	int myflags[2][10000];
	int *partnerflags[2][10000];
} flags;

flags *allnodes;

void OMP_dissemination_barrier(int *sense, int *parity)
{
	int round;
	int total_rounds;

	int thread_id = omp_get_thread_num();
	printf("Nitish: In thread: %d sense: %d parity: %d\n", thread_id,*sense,*parity);

	flags *localflags = allnodes + thread_id;
	total_rounds = ceil(log(NUM_THREADS)/log(2));
	
	for(round = 0; round < total_rounds; round++) {
		#pragma omp critical
		{
			*localflags->partnerflags[*parity][round] = *sense;
		}
		while(localflags->myflags[*parity][round] != *sense);
	}

	if(*parity == 1) {
		*sense = !(*sense);
	}

	*parity = 1 - *parity;
	printf("Nitish: Exiting thread %d with sense: %d parity: %d\n",thread_id,*sense,*parity);
}

void OMP_dissemination_setup(int threads)
{
	int thread;
	int round;
	int temp;
	int j;

	NUM_THREADS = threads;

	allnodes = (flags *) malloc (sizeof(flags) * NUM_THREADS);
	int total_rounds = ceil(log(NUM_THREADS)/log(2));

	for(thread = 0; thread < NUM_THREADS; thread++)
		for(round = 0; round < total_rounds; round++) {
			allnodes[thread].myflags[0][round] = allnodes[thread].myflags[1][round]=0;
	}
	
	for(thread = 0; thread < NUM_THREADS; thread++) {
		for(round = 0; round < total_rounds; round++) {
			temp = (int)ceil(pow(2, round));
			j = (thread + temp) % NUM_THREADS;
			allnodes[thread].partnerflags[0][round] = &allnodes[j].myflags[0][round];
			allnodes[thread].partnerflags[1][round] = &allnodes[j].myflags[1][round];
		}
	}
}

void OMP_dissemination_end()
{
	free(allnodes);

	return;
}