#include <stdio.h>
#include "OMP_dissemination_barrier.h"

int main(int argc, char *argv[])
{
  int threads = 4;
//  printf("Starting process %d of %d in node : %s\n", my_id, num_processes, processor_name);

  OMP_dissemination_setup(threads);
  #pragma omp parallel num_threads(threads)
  {
    int thread_sense = 0;
    int parity = 0;
    int i;
	
  	for (i = 0; i < 4; i++) {
    OMP_dissemination_barrier(&thread_sense, &parity);
		printf("Nitish: BARRIER %d COMPLETED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n",i+1);
		fflush(stdout);
  	}
  }
  fflush(stdout);
  OMP_dissemination_end();
  return 0;
}

