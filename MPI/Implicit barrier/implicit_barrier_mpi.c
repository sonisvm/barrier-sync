#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char** argv) {
	
	int BARRIER_COUNT = atoi(argv[1]);


	//Initialize the MPI environment
	MPI_Init(NULL, NULL);

	struct timeval start, end;
	gettimeofday(&start, NULL);
	
	for(int i=0; i< BARRIER_COUNT; i++){
		//Barrier
		MPI_Barrier(MPI_COMM_WORLD);
	}

	gettimeofday(&end, NULL);

	// Finalize the MPI environment.
	MPI_Finalize();	
	      
	printf(" Time taken = %lf\n", (double)((end.tv_sec * 1000000 + end.tv_usec)
                  - (start.tv_sec * 1000000 + start.tv_usec))/BARRIER_COUNT);
}
