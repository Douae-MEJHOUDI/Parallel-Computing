#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int nb;
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        nb = omp_get_num_threads();
        printf("Hello from the rank %d thread \n", id);
    }
    printf("Parallel execution of hello world with %d threads \n", nb);
    return 0;
}