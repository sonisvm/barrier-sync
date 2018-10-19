#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

int main(int argc, char *argv[]){
  int NUM_THREADS = atoi(argv[1]);

  omp_set_num_threads(NUM_THREADS);
  int count = NUM_THREADS;
  srand(time(0));
  clock_t arrivalTimes[NUM_THREADS];
  clock_t departTimes[NUM_THREADS];
  int n = NUM_THREADS * 1000;
  int sum =0;
  #pragma omp parallel
  {
    #pragma omp for nowait
    for (int i = 0; i < n; i++) {
      sum += i;
    }

    #pragma omp atomic
      count--;

    int id = omp_get_thread_num();
    arrivalTimes[id] = clock();
    printf("Thread %d reached the barrier with count %d\n",id, count);
    if(count==0){
      count = NUM_THREADS;
      departTimes[id] = clock();
      printf("Thread %d leaving the barrier with count %d\n", id, count);
    } else {
      while(count!=NUM_THREADS);
      departTimes[id] = clock();
      printf("Thread %d leaving the barrier with count %d\n", id, count);
    }

  }
  long int minTime=INT_MAX, maxTime = INT_MIN;
  for (int i = 0; i < NUM_THREADS; i++) {
    if(arrivalTimes[i] < minTime){
      minTime = arrivalTimes[i];
    }
    if(departTimes[i] > maxTime){
      maxTime = departTimes[i];
    }

  }
printf("time taken %lf\n", (double)(maxTime-minTime)/CLOCKS_PER_SEC);
  return 0;
}
