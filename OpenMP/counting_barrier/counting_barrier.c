#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>


int main(int argc, char **argv)
{
  int total_barrier_count = atoi(argv[2]);
  int NUM_THREADS = atoi(argv[1]);
  omp_set_num_threads(NUM_THREADS);
  int count = NUM_THREADS;
  int sense = 1;
  int local_sense = 1;
  struct timeval start, end;
  int i = total_barrier_count;
	gettimeofday(&start, NULL);

while(i) {
  #pragma omp parallel num_threads(NUM_THREADS) firstprivate(local_sense) shared(sense, count)
  {


    // loop till the last barrier
    
      local_sense = !local_sense;

      // decrementing count
      #pragma omp atomic
        --count;

      // last thread is here
      // resetting count
      // reversing sense
      if(count == 0){
        count = NUM_THREADS;
        sense = local_sense;
        
      } else {
        // wait for sense to reverse

        while(sense != local_sense);
      }

    }
 	i--;
  }
	gettimeofday(&end, NULL);

  // End of program
 printf(" Time taken = %lf\n", (double)((end.tv_sec * 1000000 + end.tv_usec)
                  - (start.tv_sec * 1000000 + start.tv_usec))/total_barrier_count);
  return 0;
}

