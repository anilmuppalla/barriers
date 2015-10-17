#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int NUMTHREADS=0, NUMBARRIERS=0, SENSE=1, COUNT;

int main( int argc, char** argv){
	if (argc != 3){
		printf("Syntax : ./sensereversal num_threads num_barriers");
		return 1;
	}
		//set the global thread and barrier counters
		NUMTHREADS = atoi(*(argv+1));
		NUMBARRIERS = atoi(*(argv+2));

		//Store the Start and End times of threads
		double threadStartTime[NUMBARRIERS][NUMTHREADS];
		double threadEndTime[NUMBARRIERS][NUMTHREADS];
		
		//set initial count = NUMTHREADS
		COUNT = NUMTHREADS;

		// Parallel construct that is executed by each thread
		#pragma omp parallel num_threads(NUMTHREADS) shared(SENSE, COUNT)
		{
			//int local_sense = 1;
			int barrier_count = 0;
			while(barrier_count < NUMBARRIERS){
		/*		printf("Thread# %d Entering Critical Section at Barrier# %d\n", 
					omp_get_thread_num(), barrier_count); */
				threadStartTime[barrier_count][omp_get_thread_num()] = omp_get_wtime();
				#pragma omp barrier
				//central_barrier(&local_sense);
				threadEndTime[barrier_count][omp_get_thread_num()] = omp_get_wtime();	
				/* printf("Thread# %d Leaving Critical Section at Barrier# %d\n", 
					omp_get_thread_num(), barrier_count); */
				barrier_count++;
			} 
		}

		//Print the Average time taken by each thread across all barriers 
		float sum=0.0, thread_avg=0.0;
		int t=0, b=0;
		while(t<NUMTHREADS){
			b=0;
			while(b<NUMBARRIERS){
				sum = sum + (threadEndTime[b][t] - threadStartTime[b][t]);
				b++;
			}
			thread_avg += sum /(float)NUMBARRIERS;
			t++;
		}

		printf("Average time taken by %d Threads in %d Barriers : %fs\n", 
			NUMTHREADS, NUMBARRIERS, thread_avg/(float)NUMTHREADS);
	return 0;	
}