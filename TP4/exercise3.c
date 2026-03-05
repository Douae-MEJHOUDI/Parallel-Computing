#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void task_light(int N) {
    double x = 0.0;
    for (int i = 0; i < N; i++) {
        x += sin(i * 0.001);
    }
}

void task_moderate(int N) {
    double x = 0.0;
    for (int i = 0; i < 5*N; i++) {
        x += sqrt(i * 0.5) * cos(i * 0.001);
    }
}

void task_heavy(int N) {
    double x = 0.0;
    for (int i = 0; i < 20*N; i++) {
        x += sqrt(i * 0.5) * cos(i * 0.001) * sin(i * 0.0001);
    }
}

int main() {

    double start, end;
    int N = 10000000;

    start = omp_get_wtime();

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            task_light(N);
        }

        #pragma omp section
        {
            task_moderate(N);
        }

        #pragma omp section
        {
            task_heavy(N);
        }
    }   
    /*
    task_light(N);
    task_moderate(N);
    task_heavy(N);*/
    end = omp_get_wtime();
    printf("Time: %f\n", end - start);
    
    return 0;
}
