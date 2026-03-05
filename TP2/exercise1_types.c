#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

int main() {
    double start, end;

    // ============== DOUBLE ==============
    printf("=== DOUBLE (8 bytes) ===\n");
    double *arr_d = malloc(N * sizeof(double));
    double sum_d;
    for (int i = 0; i < N; i++) arr_d[i] = 1.0;

    sum_d = 0.0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i++) sum_d += arr_d[i];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=1: Sum = %.0f, Time = %.4f ms\n", sum_d, (end - start) * 1000);

    sum_d = 0.0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i += 8)
        sum_d += arr_d[i] + arr_d[i+1] + arr_d[i+2] + arr_d[i+3]
               + arr_d[i+4] + arr_d[i+5] + arr_d[i+6] + arr_d[i+7];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=8: Sum = %.0f, Time = %.4f ms\n", sum_d, (end - start) * 1000);
    free(arr_d);

    // ============== FLOAT ==============
    printf("\n=== FLOAT (4 bytes) ===\n");
    float *arr_f = malloc(N * sizeof(float));
    float sum_f;
    for (int i = 0; i < N; i++) arr_f[i] = 1.0f;

    sum_f = 0.0f;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i++) sum_f += arr_f[i];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=1: Sum = %.0f, Time = %.4f ms\n", sum_f, (end - start) * 1000);

    sum_f = 0.0f;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i += 8)
        sum_f += arr_f[i] + arr_f[i+1] + arr_f[i+2] + arr_f[i+3]
               + arr_f[i+4] + arr_f[i+5] + arr_f[i+6] + arr_f[i+7];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=8: Sum = %.0f, Time = %.4f ms\n", sum_f, (end - start) * 1000);
    free(arr_f);

    // ============== INT ==============
    printf("\n=== INT (4 bytes) ===\n");
    int *arr_i = malloc(N * sizeof(int));
    long long sum_i;
    for (int i = 0; i < N; i++) arr_i[i] = 1;

    sum_i = 0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i++) sum_i += arr_i[i];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=1: Sum = %lld, Time = %.4f ms\n", sum_i, (end - start) * 1000);

    sum_i = 0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i += 8)
        sum_i += arr_i[i] + arr_i[i+1] + arr_i[i+2] + arr_i[i+3]
               + arr_i[i+4] + arr_i[i+5] + arr_i[i+6] + arr_i[i+7];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=8: Sum = %lld, Time = %.4f ms\n", sum_i, (end - start) * 1000);
    free(arr_i);

    // ============== SHORT ==============
    printf("\n=== SHORT (2 bytes) ===\n");
    short *arr_s = malloc(N * sizeof(short));
    long long sum_s;
    for (int i = 0; i < N; i++) arr_s[i] = 1;

    sum_s = 0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i++) sum_s += arr_s[i];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=1: Sum = %lld, Time = %.4f ms\n", sum_s, (end - start) * 1000);

    sum_s = 0;
    start = (double)clock() / CLOCKS_PER_SEC;
    for (int i = 0; i < N; i += 8)
        sum_s += arr_s[i] + arr_s[i+1] + arr_s[i+2] + arr_s[i+3]
               + arr_s[i+4] + arr_s[i+5] + arr_s[i+6] + arr_s[i+7];
    end = (double)clock() / CLOCKS_PER_SEC;
    printf("U=8: Sum = %lld, Time = %.4f ms\n", sum_s, (end - start) * 1000);
    free(arr_s);

    return 0;
}
