#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Version 2: schedule(dynamic) + nowait

int main(void)
{
    const int n = 40000;
    const int m = 600;
    double flops = 2.0 * n * m;

    double *mat = malloc(n * m * sizeof(double));
    double *rhs = malloc(n * sizeof(double));
    double *lhs = malloc(m * sizeof(double));

    for (int c = 0; c < n; ++c) {
        rhs[c] = 1.0;
        for (int r = 0; r < m; ++r)
            mat[r + c*m] = 1.0;
    }

    int threads[] = {1, 2, 4, 8, 16};
    double t_serial = 0.0;

    for (int t = 0; t < 5; t++) {
        omp_set_num_threads(threads[t]);

        for (int r = 0; r < m; ++r)
            lhs[r] = 0.0;

        double start = omp_get_wtime();

        #pragma omp parallel
        {
            #pragma omp for schedule(dynamic) nowait
            for (int r = 0; r < m; ++r) {
                for (int c = 0; c < n; ++c)
                    lhs[r] += mat[r + c*m] * rhs[c];
            }
        }

        double time = omp_get_wtime() - start;

        if (threads[t] == 1)
            t_serial = time;

        double speedup = t_serial / time;
        double efficiency = speedup / threads[t];
        double mflops = flops / (time * 1e6);

        int correct = 1;
        for (int r = 0; r < m; ++r)
            if (lhs[r] != (double)n) { correct = 0; break; }

        printf("Threads=%d  Time=%f  Speedup=%.2f  Efficiency=%.2f  MFLOP/s=%.2f  Check=%s\n",
               threads[t], time, speedup, efficiency, mflops, correct ? "OK" : "WRONG");
    }

    free(mat);
    free(rhs);
    free(lhs);
    return 0;
}
