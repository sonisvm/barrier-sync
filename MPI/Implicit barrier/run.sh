#!/bin/bash

START_TIME=$(date +%s.%N)
for run in {1..5}
do
	mpirun -np 2 implicit_barrier_mpi
done
END=$(date +%s.%N)

