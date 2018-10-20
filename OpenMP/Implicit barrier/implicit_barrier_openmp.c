#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <sys/time.h>

int main(int argc, char *argv[]){

	int NUM_THREADS = atoi(argv[1]);


        int BARRIER_COUNT = atoi(argv[2]);

	struct timeval start, end;

  	omp_set_num_threads(NUM_THREADS);

  	gettimeofday(&start, NULL);
  	for(int i=0; i < BARRIER_COUNT; i++){
       		#pragma omp parallel
        	{
        	}
        
  	}
 	gettimeofday(&end, NULL);
  	printf(" Time taken = %lf\n", (double)((end.tv_sec * 1000000 + end.tv_usec)
                  - (start.tv_sec * 1000000 + start.tv_usec))/BARRIER_COUNT);
  return 0;
}
