#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	int BARRIER_COUNT = 5;
	
	if(argc > 1){
		BARRIER_COUNT = atoi(argv[1]);
	}

	//Initialize the MPI environment
	MPI_Init(NULL, NULL);

	clock_t t=clock();
	
	//Barrier
	MPI_Barrier(MPI_COMM_WORLD);

	t = clock() - t;

	// Finalize the MPI environment.
	MPI_Finalize();	
	      
	printf("Time taken = %lf\n", ((double)t/CLOCKS_PER_SEC)/BARRIER_COUNT); 
}
