#undef MPI_OMP_COMBINED
#undef MPI_TOURNAMENT
#undef MPI_SENSE_REVERSAL
#undef OMP_DISSEMINATION
#undef OMP_SENSE_REVERSAL
#define DEFAULT_MPI
#undef DEFAULT_OMP

#include <stdio.h>
#include <stdlib.h>

#ifdef MPI_OMP_COMBINED
#include "MPI_tournament_barrier.h"
#include "OMP_dissemination_barrier.h"
#endif

#if defined(OMP_DISSEMINATION) || defined(DEFAULT_OMP)
#include "OMP_dissemination_barrier.h"
#endif

#ifdef MPI_TOURNAMENT
#include "MPI_tournament_barrier.h"
#endif

#if defined(MPI_SENSE_REVERSAL) || defined(DEFAULT_MPI)
#include "MPI_sense_reversal_barrier.h"
#endif

#ifdef OMP_SENSE_REVERSAL
#include "OMP_sense_reversal_barrier.h"
#endif

int main(int argc, char *argv[])
{

  int sense = 1;
  int threads, barriers;
  int j,k;
  int rank;

#if defined(MPI_OMP_COMBINED) || defined(OMP_DISSEMINATION) || defined(OMP_SENSE_REVERSAL) || defined(DEFAULT_OMP)
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
  MPI_Comm_size(MPI_COMM_WORLD, &rank);
#endif
#ifdef OMP_DISSEMINATION
  OMP_dissemination_setup(threads);
#endif
#ifdef OMP_SENSE_REVERSAL
  OMP_sense_reversal_setup(threads);
#endif
#endif

#if defined(MPI_TOURNAMENT) || defined(MPI_SENSE_REVERSAL) || defined(DEFAULT_MPI)
  barriers = atoi(argv[1]);

  double *threadStartTime, *threadEndTime, average, sum;

  threadStartTime = (double *)malloc (barriers * sizeof(double));
  threadEndTime = (double *)malloc (barriers * sizeof(double));
#ifdef MPI_SENSE_REVERSAL
  MPI_sense_reversal_setup(MPI_COMM_WORLD);
  MPI_Comm_size(MPI_COMM_WORLD, &rank);
#endif
#if defined(MPI_TOURNAMENT) || defined(DEFAULT_MPI)
  MPI_tournament_setup(MPI_COMM_WORLD);
  MPI_Comm_size(MPI_COMM_WORLD, &rank);
#endif
#endif

#if defined(MPI_OMP_COMBINED) || defined(OMP_DISSEMINATION) || defined(OMP_SENSE_REVERSAL)  || defined(DEFAULT_OMP)
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
#ifdef OMP_SENSE_REVERSAL
		threadStartTime[omp_get_thread_num()][i] = omp_get_wtime();
		OMP_sense_reversal_barrier(&thread_sense,&sense);
		threadEndTime[omp_get_thread_num()][i] = omp_get_wtime();
#endif
#ifdef DEFAULT_OMP
		threadStartTime[omp_get_thread_num()][i] = omp_get_wtime();
		{
			#pragma omp barrier
		}
		threadEndTime[omp_get_thread_num()][i] = omp_get_wtime();
#endif
#ifdef MPI_TOURNAMENT
		threadStartTime[i] = MPI_Wtime();
		MPI_tournament_barrier();
		threadEndTime[i] = MPI_Wtime();
#endif
#ifdef MPI_SENSE_REVERSAL
		threadStartTime[i] = MPI_Wtime();
		MPI_sense_reversal_barrier();
		threadEndTime[i] = MPI_Wtime();
#endif
#ifdef DEFAULT_MPI
		threadStartTime[i] = MPI_Wtime();
		MPI_Barrier(MPI_COMM_WORLD);
		threadEndTime[i] = MPI_Wtime();
#endif

	}
#if defined(MPI_OMP_COMBINED) || defined(OMP_DISSEMINATION) || defined(OMP_SENSE_REVERSAL)  || defined(DEFAULT_OMP)
  }
#endif
  average = 0;

#if defined(MPI_OMP_COMBINED) || defined(OMP_DISSEMINATION) || defined(OMP_SENSE_REVERSAL)  || defined(DEFAULT_OMP)
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
#ifdef OMP_SENSE_REVERSAL
  printf("OMP_Sense_Reversal_Barrier: Threads: %d Barriers: %d Average: %lf\n",threads, barriers, average);
#endif
#ifdef DEFAULT_OMP
  printf("OMP_Default_Barrier: Threads: %d Barriers: %d Average: %lf\n",threads, barriers, average);
#endif
#endif

#if defined(MPI_TOURNAMENT) || defined(MPI_SENSE_REVERSAL) || defined(DEFAULT_MPI)
	sum = 0;

	for (j=0;j<barriers;j++) {
		sum += (threadEndTime[j] - threadStartTime[j]);
	}

	average = (sum / barriers);
#ifdef MPI_TOURNAMENT
	printf("MPI_Tournament_Barrier: Processes: %d Barriers: %d Average: %lf\n",rank,barriers, average);
#endif
#ifdef MPI_SENSE_REVERSAL
	printf("MPI_Sense_Reversal_Barrier: Processes: %d Barriers: %d Average: %lf\n",rank,barriers, average);
#endif
#ifdef DEFAULT_MPI
	printf("MPI_Default_Barrier: Processes: %d Barriers: %d Average: %lf\n",rank,barriers, average);
#endif
#endif

#if defined(MPI_OMP_COMBINED) || defined(OMP_DISSEMINATION) || defined(OMP_SENSE_REVERSAL)
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
#ifdef OMP_SENSE_REVERSAL
  OMP_sense_reversal_end();
#endif
#if defined(MPI_TOURNAMENT) || defined(DEFAULT_MPI)
  MPI_tournament_end();
#endif
#ifdef MPI_SENSE_REVERSAL
  MPI_sense_reversal_end();
#endif

  return 0;
}

