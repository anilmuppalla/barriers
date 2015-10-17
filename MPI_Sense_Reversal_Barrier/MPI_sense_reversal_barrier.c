#include <stdio.h>
#include "MPI_sense_reversal_barrier.h"


int sense;
int num_processes;
int process_id;

void MPI_sense_reversal_setup(MPI_Comm comm)
{
	MPI_Init(NULL, NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

	sense = 0;

	return;
}

void MPI_sense_reversal_barrier()
{
	int i;
	int my_msg = 1;
	int priv_sense = !sense;

	if (process_id == num_processes - 1) {

		for (i = 0; i < num_processes - 1; i++)		//LAST process is waiting on all other processes
			MPI_Recv(&my_msg, 1, MPI_INT, i, 1, MPI_COMM_WORLD, NULL);

		sense = !sense;

		MPI_Bcast(&sense, 1, MPI_INT, process_id, MPI_COMM_WORLD); //LAST process is waiting on all other processes

	}

	else {

		MPI_Send(&my_msg, 1, MPI_INT, num_processes - 1, 1, MPI_COMM_WORLD);
		
		MPI_Bcast(&sense, 1, MPI_INT, num_processes - 1, MPI_COMM_WORLD);

	}

	return;
}

void MPI_sense_reversal_end()
{
	MPI_Finalize();
}
