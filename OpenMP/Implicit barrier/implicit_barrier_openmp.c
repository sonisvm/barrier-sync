#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

int main(int argc, char *argv[]){
  	int NUM_THREADS = 2;
	int BARRIER_COUNT = 5;
	
	if(argc > 1){
		NUM_THREADS = atoi(argv[1]);
	}
	if(argc > 2){
		BARRIER_COUNT = atoi(argv[2]);
	}

  omp_set_num_threads(NUM_THREADS);

  clock_t t = clock();

  for(int i=0; i < BARRIER_COUNT; i++){
	#pragma omp parallel
	{
	}
	
  }

  t = clock()-t;
  printf("Time taken = %lf\n", ((double)t/CLOCKS_PER_SEC)/BARRIER_COUNT);

  return 0;
}
