#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

int num;
int rank;
int *partner_flags;

int num_rounds;

void dissemination_barrier() {
	int send_buff;
	int rec_buff;
	MPI_Status stat;

	for(int i = 0; i < num_rounds; i++) {
		MPI_Send(&send_buff, 1, MPI_INT, partner_flags[i], i, MPI_COMM_WORLD);
		MPI_Recv(&rec_buff, 1, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &stat);
  	}
}

int main(int argc, char *argv[]) {
	int init;
  	num = atoi(argv[1]);

	init = MPI_Init(&argc, &argv);
	if (init != MPI_SUCCESS) {
		MPI_Abort(MPI_COMM_WORLD, init);
	}

	//dissemination_barrier_init();
  	MPI_Comm_size(MPI_COMM_WORLD, &num);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	num_rounds = (int) ceil(log2(num));
  	partner_flags = (int*) calloc(num_rounds, sizeof(int));
  	int ex = 1;

 	for(int i = 0; i < num_rounds; i++) {
      		partner_flags[i] = (rank + ex) % num;
      		ex = ex * 2;
  	}
	
	struct timeval start, end;
	gettimeofday(&start, NULL);

  	for(int j = 0; j < num; j++) {
    		dissemination_barrier(rank);
  	}

	gettimeofday(&end, NULL);
	MPI_Barrier(MPI_COMM_WORLD);

	printf("Time taken for process %d = %lf\n", rank, (double)((end.tv_sec * 1000000 + end.tv_usec)
                  - (start.tv_sec * 1000000 + start.tv_usec))/num);

  	MPI_Finalize();
	return 0;
}

