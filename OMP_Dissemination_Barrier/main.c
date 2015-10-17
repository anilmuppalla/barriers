#undef MPI_OMP_COMBINED
#undef MPI_TOURNAMENT
#define OMP_DISSEMINATION

#include <stdio.h>
#include <stdlib.h>

#ifdef MPI_OMP_COMBINED
#include "MPI_tournament_barrier.h"
#include "OMP_dissemination_barrier.h"
#endif

#ifdef OMP_DISSEMINATION
#include "OMP_dissemination_barrier.h"
#endif

#ifdef MPI_TOURNAMENT
#include "MPI_tournament_barrier.h"
#endif

int main(int argc, char *argv[])
{

  int sense = 0;
  int threads, barriers;
  int j,k;

#if defined(MPI_OMP_COMBINED) || defined(OMP_DISSEMINATION)
  threads = atoi(argv[1]);
  barriers = atoi(argv[2]);

  double **threadStartTime, **threadEndTime, average, sum;

  threadStartTime = (double **)malloc (threads * sizeof (double *));
  threadEndTime = (double **)malloc (threads * sizeof (double *));

  for (j=0; j<threads; j++) {
  	threadStartTime[j] = (double *)malloc (barriers * sizeof(double));
	threadEndTime[j] = (double *)malloc (barriers * sizeof(double));
  }
#ifdef MPI_OMP_COMBINED
  MPI_combined_barrier_setup(MPI_COMM_WORLD, threads);
#endif
#ifdef OMP_DISSEMINATION
  OMP_dissemination_setup(threads);
#endif
#endif

#ifdef MPI_TOURNAMENT
  barriers = atoi(argv[1]);

  double *threadStartTime, *threadEndTime, average, sum;

  threadStartTime = (double *)malloc (barriers * sizeof(double));
  threadEndTime = (double *)malloc (barriers * sizeof(double));

  MPI_tournament_setup(MPI_COMM_WORLD);
#endif

#if defined(MPI_OMP_COMBINED) || defined(OMP_DISSEMINATION)
  #pragma omp parallel num_threads(threads) shared(sense)
  {
#endif
    int thread_sense = 1;
	int parity = 0;
	int i;
	
	
	for (i = 0; i < barriers; i++) {
#ifdef MPI_OMP_COMBINED
		threadStartTime[omp_get_thread_num()][i] = omp_get_wtime();
		MPI_combined_barrier(&thread_sense,&parity,&sense);
		threadEndTime[omp_get_thread_num()][i] = omp_get_wtime();
#endif
#ifdef OMP_DISSEMINATION
		threadStartTime[omp_get_thread_num()][i] = omp_get_wtime();
		OMP_dissemination_barrier(&thread_sense,&parity);
		threadEndTime[omp_get_thread_num()][i] = omp_get_wtime();
#endif
#ifdef MPI_TOURNAMENT
		threadStartTime[i] = MPI_Wtime();
		MPI_tournament_barrier();
		threadEndTime[i] = MPI_Wtime();
#endif
		
	}
#if defined(MPI_OMP_COMBINED) || defined(OMP_DISSEMINATION)
  }
#endif
  average = 0;

#if defined(MPI_OMP_COMBINED) || defined(OMP_DISSEMINATION)
  for (j=0;j<threads;j++) {
  	sum = 0;
  	for (k=0;k<barriers;k++)
		sum += (threadEndTime[j][k] - threadStartTime[j][k]);
	average += (sum / barriers);
  }
  average = average / threads;
#ifdef MPI_OMP_COMBINED
  printf("MPIOMP_Combined_Barrier: Threads: %d Barriers: %d Average: %lf\n",threads, barriers, average);
#endif
#ifdef OMP_DISSEMINATION
  printf("OMP_Dissemination_Barrier: Threads: %d Barriers: %d Average: %lf\n",threads, barriers, average);
#endif
#endif

#ifdef MPI_TOURNAMENT
	sum = 0;

	for (j=0;j<barriers;j++) {
		sum += (threadEndTime[j] - threadStartTime[j]);
	}

	average = (sum / barriers);
	printf("MPI_Tournament_Barrier: Barriers: %d Average: %lf\n",barriers, average);
#endif

#if defined(MPI_OMP_COMBINED) || defined(OMP_DISSEMINATION)
  for (j=0; j<threads; j++) {
  	free(threadStartTime[j]);
	free(threadEndTime[j]);
  }
#endif

  free(threadStartTime);
  free(threadEndTime);

  fflush(stdout);

#ifdef MPI_OMP_COMBINED
  MPI_combined_barrier_end();
#endif
#ifdef OMP_DISSEMINATION
  OMP_dissemination_end();
#endif
#ifdef MPI_TOURNAMENT
  MPI_tournament_end();
#endif

  return 0;
}

