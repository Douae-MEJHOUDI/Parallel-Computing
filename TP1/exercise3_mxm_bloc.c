#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000
#define MAX_BLOCK_SIZE 20

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

void mxm_block(double **a, double **b, double **c, int n, int block_size) {
    int i, j, k, ii, jj, kk;

    for (ii = 0; ii < n; ii += block_size) {
        for (jj = 0; jj < n; jj += block_size) {
            for (kk = 0; kk < n; kk += block_size) {
                // Multiply block
                for (i = ii; i < ii + block_size && i < n; i++) {
                    for (j = jj; j < jj + block_size && j < n; j++) {
                        for (k = kk; k < kk + block_size && k < n; k++) {
                            c[i][j] += a[i][k] * b[k][j];
                        }
                    }
                }
            }
        }
    }
}

int main() {
    double **a, **b, **c;
    double start, end, msec, rate;
    long long ops = 2LL * N * N * N;
    double bytes = 3.0 * N * N * sizeof(double);
    int block_sizes[] = {1, 2, 4, 5, 8, 10, 16, 20, 25, 32, 40, 50};
    int num_blocks = sizeof(block_sizes) / sizeof(block_sizes[0]);

    srand(42);

    a = allocate_matrix(N);
    b = allocate_matrix(N);
    c = allocate_matrix(N);

    init_matrix(a, N);
    init_matrix(b, N);

    printf("Matrix size: %d x %d\n", N, N);
    printf("Block Matrix Multiplication Performance\n");
    printf("=====================================================\n");
    printf("Block Size, Time (ms), Bandwidth (MB/s), GFLOPS\n");
    printf("=====================================================\n");

    for (int b_idx = 0; b_idx < num_blocks; b_idx++) {
        int block_size = block_sizes[b_idx];

        zero_matrix(c, N);

        start = (double)clock() / CLOCKS_PER_SEC;
        mxm_block(a, b, c, N, block_size);
        end = (double)clock() / CLOCKS_PER_SEC;

        msec = (end - start) * 1000.0;
        rate = bytes * (1000.0 / msec) / (1024 * 1024);

        printf("%d, %.2f, %.2f, %.2f\n",
               block_size, msec, rate, ops / (msec / 1000.0) / 1e9);
    }

    printf("=====================================================\n");

    free_matrix(a, N);
    free_matrix(b, N);
    free_matrix(c, N);

    return 0;
}
