#!/bin/bash
gcc -fopenmp ex4_schedule.c -o ex4_schedule

export OMP_NUM_THREADS=36

echo "=== STATIC ==="
for chunk in 1 10 50 100; do
    echo -n "static,$chunk: "
    OMP_SCHEDULE="static,$chunk" ./ex4_schedule
done

echo "=== DYNAMIC ==="
for chunk in 1 10 50 100; do
    echo -n "dynamic,$chunk: "
    OMP_SCHEDULE="dynamic,$chunk" ./ex4_schedule
done

echo "=== GUIDED ==="
for chunk in 1 10 50 100; do
    echo -n "guided,$chunk: "
    OMP_SCHEDULE="guided,$chunk" ./ex4_schedule
done
