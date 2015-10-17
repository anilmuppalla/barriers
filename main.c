#include <stdio.h>
#include <stdlib.h>
#include "MPI_tournament_barrier.h"
#include "OMP_dissemination_barrier.h"


int main(int argc, char *argv[])
{

  int sense = 0;
  int threads, barriers;
  int j,k;

  threads = atoi(argv[1]);
  barriers = atoi(argv[2]);

  double **threadStartTime, **threadEndTime, average, sum;
  
  threadStartTime = (double **)malloc (threads * sizeof (double *));
  threadEndTime = (double **)malloc (threads * sizeof (double *));

  for (j=0; j<threads; j++) {
  	threadStartTime[j] = (double *)malloc (barriers * sizeof(double));
	threadEndTime[j] = (double *)malloc (barriers * sizeof(double));
  }

  MPI_combined_barrier_setup(MPI_COMM_WORLD, threads);

  #pragma omp parallel num_threads(threads) shared(sense)
  {
    int thread_sense = 1;
	int parity = 0;
	int i;
	
	
	for (i = 0; i < barriers; i++) {
		threadStartTime[omp_get_thread_num()][i] = omp_get_wtime();
		MPI_combined_barrier(&thread_sense,&parity,&sense);
		threadEndTime[omp_get_thread_num()][i] = omp_get_wtime();
	}
  	
  }

  average = 0;
  for (j=0;j<threads;j++) {
  	sum = 0;
  	for (k=0;k<barriers;k++)
		sum += (threadEndTime[j][k] - threadStartTime[j][k]);
	average += (sum / barriers);
  }

  average = average / threads;

  printf("MPIOMP_Combined_Barrier: Threads: %d Barriers: %d Average: %lf\n",threads, barriers, average);

  for (j=0; j<threads; j++) {
  	free(threadStartTime[j]);
	free(threadEndTime[j]);
  }

  free(threadStartTime);
  free(threadEndTime);

  fflush(stdout);
  MPI_combined_barrier_end();

  return 0;
}

