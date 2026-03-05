#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 1000000

int main() {

    double *A = malloc(N * sizeof(double));
    if (A == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    double sum = 0.0;
    double mean = 0.0;
    double stddev = 0.0;
    double max;

    // Initialization
    srand(0);
    for (int i = 0; i < N; i++)
        A[i] = (double)rand() / RAND_MAX;

    max = A[0];

    // Phase 1: Compute sum and max in parallel
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // Section 1: Compute the sum
            for (int i = 0; i < N; i++)
                sum += A[i];
        }

        #pragma omp section
        {
            // Section 2: Compute the maximum value
            for (int i = 1; i < N; i++) {
                if (A[i] > max)
                    max = A[i];
            }
        }
    }
    // Implicit barrier: sum and max are ready

    mean = sum / N;

    // Phase 2: Compute standard deviation (reuses sum via mean)
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // Section 3: Compute the standard deviation
            for (int i = 0; i < N; i++)
                stddev += (A[i] - mean) * (A[i] - mean);

            stddev = sqrt(stddev / N);
        }
    }

    // Print results
    printf("Sum     = %f\n", sum);
    printf("Max     = %f\n", max);
    printf("Std Dev = %f\n", stddev);

    free(A);
    return 0;
}
