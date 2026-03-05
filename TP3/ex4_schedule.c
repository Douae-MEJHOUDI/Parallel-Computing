#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int m = 1000;
    int n = 1000;
    double itime, ftime;

    double *a = (double *)malloc(m * n * sizeof(double));
    double *b = (double *)malloc(n * m * sizeof(double));
    double *c = (double *)malloc(m * m * sizeof(double));

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            a[i * n + j] = (i + 1) + (j + 1);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            b[i * m + j] = (i + 1) - (j + 1);

    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            c[i * m + j] = 0;

    itime = omp_get_wtime();

    #pragma omp parallel for schedule(runtime)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < n; k++) {
                c[i * m + j] += a[i * n + k] * b[k * m + j];
            }
        }
    }

    ftime = omp_get_wtime();
    printf("Time = %.3f seconds .\n", ftime - itime);

    free(a);
    free(b);
    free(c);
    return 0;
}
