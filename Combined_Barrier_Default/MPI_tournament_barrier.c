#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MPI_tournament_barrier.h"


typedef enum roles {
	WINNER,
	LOSER,
	BYE,
	CHAMPION,
	DROPOUT,
	UNUSED
}roles;

typedef struct round
{
	roles role;
	int opponent;
	int flag;
} round_t;

static round_t **rounds;
int sense;
int num_processes;
int process_id;

void MPI_tournament_setup(MPI_Comm comm)
{
	int i, k;
	int total_rounds;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(comm, &num_processes);
	MPI_Comm_rank(comm, &process_id);
	MPI_Get_processor_name(processor_name, &name_len);
	sense = 0;
	total_rounds = ceil(log2(num_processes)) + 1;
	
	//printf("Starting process %d of %d in node : %s\n", process_id, num_processes, processor_name);

	rounds = (round_t **) malloc (num_processes * sizeof(round_t *));

	for (i = 0;i < num_processes; i++) {
		rounds[i] = (round_t *) malloc (total_rounds * sizeof (round_t));
	}

	for (i=0 ; i<num_processes ; i++) {
		for (k = 0; k< total_rounds; k++) {
			rounds[i][k].flag = 0;

			if (k > 0) {

				if ((int)fmod(i,pow(2,k)) == 0 && (i + pow(2,k-1)) < num_processes && pow(2,k) < num_processes)
					rounds[i][k].role = WINNER;

				else if ((int)fmod(i,pow(2,k)) == 0 && (i + pow(2,k-1)) >= num_processes)
					rounds[i][k].role = BYE;

				else if ((int)fmod(i,pow(2,k)) == pow(2,k-1))
					rounds[i][k].role = LOSER;

				else if (i == 0 && pow(2,k) >= num_processes)
					rounds[i][k].role = CHAMPION;

				else
					rounds[i][k].role = UNUSED;
			}
			else
				rounds[i][k].role = DROPOUT;

			switch(rounds[i][k].role) {
				case LOSER:
					rounds[i][k].opponent = i - pow(2,k-1);
					break;
				case WINNER:
				case CHAMPION:
					rounds[i][k].opponent = i + pow(2,k-1);
				default:
					break;
			}
		}
	}

	return;
}

void MPI_tournament_end()
{
	int i;

	for (i = 0;i < num_processes; i++)
			free(rounds[i]);

	free(rounds);

	MPI_Finalize();

	return;
}

void MPI_tournament_barrier()
{
	int cur_round = 1;
	int loop_exit = 0;
	int total_rounds = ceil(log2(num_processes)) + 1;
	sense = !(sense);

	do {
		switch (rounds[process_id][cur_round].role) {

			case LOSER:
				
				MPI_Send(&sense, 1, MPI_INT, rounds[process_id][cur_round].opponent, 1, MPI_COMM_WORLD);
				MPI_Recv(&rounds[process_id][cur_round].flag, 1, MPI_INT, rounds[process_id][cur_round].opponent, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);

				loop_exit = 1;

				break;

			case WINNER:

				MPI_Recv(&rounds[process_id][cur_round].flag, 1, MPI_INT, rounds[process_id][cur_round].opponent, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);

				break;

			case CHAMPION:

				MPI_Recv(&rounds[process_id][cur_round].flag, 1, MPI_INT, rounds[process_id][cur_round].opponent, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
				MPI_Send(&sense, 1, MPI_INT, rounds[process_id][cur_round].opponent, 1, MPI_COMM_WORLD);

				loop_exit = 1;

			default:
				break;
			}

		cur_round += !loop_exit;	

	} while (!loop_exit && cur_round <= total_rounds);

	loop_exit = 0;

	while (--cur_round) {
		switch (rounds[process_id][cur_round].role) {
			case WINNER:

				MPI_Send(&sense, 1, MPI_INT, rounds[process_id][cur_round].opponent, 1, MPI_COMM_WORLD);
				break;

			case DROPOUT:
				loop_exit = 1;

			default:
				break;
		}

		if (loop_exit)
			break;
	}

	return;
}

