#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000

double** allocate_matrix(int n) {
    double **mat = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        mat[i] = (double*)malloc(n * sizeof(double));
    }
    return mat;
}

void free_matrix(double **mat, int n) {
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

void init_matrix(double **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = (double)rand() / RAND_MAX;
        }
    }
}

void zero_matrix(double **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = 0.0;
        }
    }
}

void mxm_ijk(double **a, double **b, double **c, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void mxm_ikj(double **a, double **b, double **c, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main() {
    double **a, **b, **c;
    double start, end, msec, rate;
    long long ops = 2LL * N * N * N; // Number of floating point operations
    double bytes = 3.0 * N * N * sizeof(double); // Memory accessed

    srand(42);

    a = allocate_matrix(N);
    b = allocate_matrix(N);
    c = allocate_matrix(N);

    init_matrix(a, N);
    init_matrix(b, N);

    printf("Matrix size: %d x %d\n", N, N);
    printf("===========================================\n\n");

    zero_matrix(c, N);
    start = (double)clock() / CLOCKS_PER_SEC;
    mxm_ijk(a, b, c, N);
    end = (double)clock() / CLOCKS_PER_SEC;
    msec = (end - start) * 1000.0;
    rate = bytes * (1000.0 / msec) / (1024 * 1024);
    printf("i-j-k order (standard):\n");
    printf("  Time: %.2f ms\n", msec);
    printf("  Bandwidth: %.2f MB/s\n", rate);
    printf("  GFLOPS: %.2f\n\n", ops / (msec / 1000.0) / 1e9);

    zero_matrix(c, N);
    start = (double)clock() / CLOCKS_PER_SEC;
    mxm_ikj(a, b, c, N);
    end = (double)clock() / CLOCKS_PER_SEC;
    msec = (end - start) * 1000.0;
    rate = bytes * (1000.0 / msec) / (1024 * 1024);
    printf("i-k-j order (optimized):\n");
    printf("  Time: %.2f ms\n", msec);
    printf("  Bandwidth: %.2f MB/s\n", rate);
    printf("  GFLOPS: %.2f\n\n", ops / (msec / 1000.0) / 1e9);

    free_matrix(a, N);
    free_matrix(b, N);
    free_matrix(c, N);

    return 0;
}
