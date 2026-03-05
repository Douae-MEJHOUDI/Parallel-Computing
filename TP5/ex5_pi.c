#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, nprocs;
    long long N = 1000000;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (argc == 2) N = atoll(argv[1]);

    double t1 = MPI_Wtime();

    // Each process handles every nprocs-th iteration starting from its rank
    // e.g. with 4 procs: rank0->0,4,8,...  rank1->1,5,9,...  etc.
    // This automatically handles N not divisible by nprocs
    double local_sum = 0.0;
    for (long long i = rank; i < N; i += nprocs) {
        double x = (i + 0.5) / N;
        local_sum += 1.0 / (1.0 + x * x);
    }

    // Sum all local results into total_sum on rank 0
    double total_sum = 0.0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double t2 = MPI_Wtime();

    if (rank == 0) {
        double pi = 4.0 * total_sum / N;
        printf("Pi = %.15f\n", pi);
        printf("Error = %e\n", fabs(pi - M_PI));
        printf("Time: %f seconds\n", t2 - t1);
    }

    MPI_Finalize();
    return 0;
}
