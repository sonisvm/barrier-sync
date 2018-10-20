#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Stroing start time
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds)
        ;
}

int main(int argc, char **argv)
{
  int total_barrier_count = atoi(argv[2]);
  int NUM_THREADS = atoi(argv[1]);
  int DELAY_CONSTANT = 2;
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
    int delay_time;

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
        while(sense != local_sense) {
	  delay_time = DELAY_CONSTANT * count;
          delay(delay_time);
        }
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

