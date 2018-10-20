#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

struct roundInfo{
  char role;
  int opponent;
  int sense;
};

int num_process;

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


void tournament_barrier(struct roundInfo rounds[][num_process], int num_rounds, int pid){
	
	int round = 0;
	while(round < num_rounds){
		switch(rounds[round][pid].role){
			case 'l':{
				int opponent = rounds[round][pid].opponent;
				//send a msg to the winner of the round
				MPI_Send(&pid, 1, MPI_INT, opponent, round, MPI_COMM_WORLD);
				//printf("P: %d, Send signal to processor in arrival %d, round=%d\n", pid, opponent, round);
				
				break;
			}
			case 'w':{
				int opponent = rounds[round][pid].opponent;
				//wait until loser signals
				int data;
				MPI_Recv(&data, 1, MPI_INT, opponent, round, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				rounds[round][pid].sense = 1;
				//printf("P: %d, Received signal from processor in arrival %d, round=%d\n",pid, opponent, round);
				
				break;
			}
			case 'c':{
				int opponent = rounds[round][pid].opponent;
				//wait until loser signals
				int data;
				MPI_Recv(&data, 1, MPI_INT, opponent, round, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				rounds[round][pid].sense = 1;
				//printf("P: %d, Received signal from processor in arrival %d, round=%d\n",pid, opponent, round);
				break;
			}
			default: break;
				
		}
		round++;
	}
	round--;
	while(round >= 0){
		switch(rounds[round][pid].role){
			case 'c':
			case 'w':{
				int opponent = rounds[round][pid].opponent;
				MPI_Send(&pid, 1, MPI_INT, opponent,round, MPI_COMM_WORLD);
				//printf("P: %d, Send signal to processor in wakeup %d\n", pid, opponent);
				break;
			}
			case 'l': {
				int opponent = rounds[round][pid].opponent;
				//block until we receive a message from winner
				int data;
				MPI_Recv(&data, 1, MPI_INT, opponent, round, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				rounds[round][pid].sense = 1;
				//printf("P: %d, Received signal from processor in wakeup %d, round=%d\n",pid, opponent, round);
				break;
			}
			default: break;
		}
		round--;
	}
	return;

}

int main(int argc, char** argv) {
	int NUM_THREADS = atoi(argv[1]);
	int BARRIER_COUNT = atoi(argv[2]);
    // Initialize the MPI environment
    int provided;
    MPI_Init_thread(NULL, NULL, MPI_THREAD_SERIALIZED, &provided);

    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);

    int num_rounds = ceil(log2(num_process));

    struct roundInfo rounds[num_rounds][num_process];

    // Get the rank of the process
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    for(int i=0; i< num_rounds; i++){
	rounds[i][process_rank].opponent = -1;
	rounds[i][process_rank].sense = 0;
    }

    //determine the winner/loser of each round
    for (int i = 0; i < num_rounds; i++) {
        int currentRound = i+1;
        if(currentRound>0 && (process_rank % (int)pow(2, currentRound)==0) && (process_rank + (int)pow(2, currentRound-1) < num_process) && ((int)pow(2, currentRound)<num_process)){
          rounds[i][process_rank].role='w';
        } else if (currentRound>0 && (process_rank % (int)pow(2, currentRound)==0) && (process_rank + (int)pow(2, currentRound-1) >= num_process)) {
          rounds[i][process_rank].role='b';
        } else if(currentRound>0 && (process_rank % (int)pow(2, currentRound) == (int)pow(2, currentRound-1))){
          rounds[i][process_rank].role='l';
        } else if (currentRound>0 && process_rank==0 && (int)pow(2, currentRound)>=num_process) {
          rounds[i][process_rank].role='c';
        } else if(currentRound==0){
          rounds[i][process_rank].role = 'd';
        } else{
          rounds[i][process_rank].role = 'u';
        }
    }


	
    for(int i=0; i<num_rounds; i++){
        switch(rounds[i][process_rank].role){
		case 'l':{
			rounds[i][process_rank].opponent = process_rank - (int)pow(2, i);
			break;
		}
		case 'w':
		case 'c':{
			rounds[i][process_rank].opponent = process_rank + (int)pow(2, i);
		}
			
	}
    }

   // for (int i = 0; i < num_rounds; i++) {
  //      printf("round=%d, process=%d, role=%c, opponent=%d\n",i, process_rank, rounds[i][process_rank].role, rounds[i][process_rank].opponent);
  //  }
    struct timeval start, end;

    
	int count = NUM_THREADS;
	int DELAY_CONSTANT = 2;
	int sense = 1;
  	int local_sense = 1;
	gettimeofday(&start, NULL);
	for(int i=0; i< BARRIER_COUNT; i++){
		#pragma omp parallel num_threads(NUM_THREADS) firstprivate(local_sense) shared(sense, count)
		{
			local_sense = !local_sense;
			int delay_time;
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
	}
	gettimeofday(&end, NULL);
	printf("Time taken in counting barrier for process %d = %lf\n", process_rank, (double)((end.tv_sec * 1000000 + end.tv_usec)
                  - (start.tv_sec * 1000000 + start.tv_usec))/BARRIER_COUNT);

    gettimeofday(&start, NULL);
	for(int i=0; i< BARRIER_COUNT; i++)
	{
		tournament_barrier(rounds, num_rounds, process_rank);
	}
    
    gettimeofday(&end, NULL);

	printf(" Time taken = %lf\n", (double)((end.tv_sec * 1000000 + end.tv_usec)
                  - (start.tv_sec * 1000000 + start.tv_usec))/BARRIER_COUNT);

    // Finalize the MPI environment.
    MPI_Finalize();
}
