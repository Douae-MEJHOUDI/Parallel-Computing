#!/bin/bash

PROCS=(1 2 4 8)

# --- Exercise 4: Matrix-Vector ---
OUTPUT4="results_ex4.csv"
echo "n_procs,N,time" > $OUTPUT4
SIZES4=(1000 2000 4000 8000 10000)

for N in "${SIZES4[@]}"; do
    for P in "${PROCS[@]}"; do
        echo "Ex4: P=$P  N=$N"
        T=$(mpirun -n $P ./ex4 $N | grep "Time:" | awk '{print $2}')
        echo "$P,$N,$T" >> $OUTPUT4
    done
done
echo "Ex4 done -> $OUTPUT4"

# --- Exercise 5: Pi Calculation ---
OUTPUT5="results_ex5.csv"
echo "n_procs,N,time" > $OUTPUT5
SIZES5=(1000000 10000000 100000000 1000000000)

for N in "${SIZES5[@]}"; do
    for P in "${PROCS[@]}"; do
        echo "Ex5: P=$P  N=$N"
        T=$(mpirun -n $P ./ex5 $N | grep "Time:" | awk '{print $2}')
        echo "$P,$N,$T" >> $OUTPUT5
    done
done
echo "Ex5 done -> $OUTPUT5"
