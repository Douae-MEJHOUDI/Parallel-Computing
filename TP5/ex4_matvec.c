#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>


void matrixVectorMult(double* A, double* b, double* x, int size) {
    for (int i = 0; i < size; ++i) {
        x[i] = 0.0;
        for (int j = 0; j < size; ++j) {
            x[i] += A[i * size + j] * b[j];
        }
    }
}

int main(int argc, char* argv[]) {
    int rank, nprocs, N;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (argc != 2) {
        if (rank == 0) printf("Usage: %s <matrix_size>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }
    N = atoi(argv[1]);
    if (N <= 0) {
        if (rank == 0) printf("Matrix size must be positive.\n");
        MPI_Finalize();
        return 1;
    }

    // --- Compute how many rows each process gets ---
    // (handles the case where N is not divisible by nprocs)
    int* sendcounts = malloc(nprocs * sizeof(int)); // number of doubles each process gets
    int* displs     = malloc(nprocs * sizeof(int)); // offset into A where each chunk starts

    int base_rows = N / nprocs;       // minimum rows per process
    int remainder = N % nprocs;       // first 'remainder' processes get one extra row

    int offset = 0;
    for (int i = 0; i < nprocs; i++) {
        int rows_i = base_rows + (i < remainder ? 1 : 0);
        sendcounts[i] = rows_i * N;   // rows_i rows, each of length N doubles
        displs[i]     = offset;
        offset       += sendcounts[i];
    }

    int my_rows = base_rows + (rank < remainder ? 1 : 0);

    // --- Memory allocation ---
    double* A        = NULL;
    double* b        = malloc(N * sizeof(double));
    double* x_serial = NULL;
    double* x_parallel = malloc(N * sizeof(double));
    double* local_A  = malloc(my_rows * N * sizeof(double));
    double* local_x  = malloc(my_rows * sizeof(double));

    if (rank == 0) {
        A        = malloc(N * N * sizeof(double));
        x_serial = malloc(N * sizeof(double));

        srand48(42);

        // Fill A[0][:100] with random values
        int limit = (N < 100) ? N : 100;
        for (int j = 0; j < limit; ++j)
            A[0 * N + j] = drand48();

        // Copy A[0][:100] into A[1][100:200] if possible
        if (N > 1 && N > 100) {
            int copy_len = (N - 100 < 100) ? (N - 100) : 100;
            for (int j = 0; j < copy_len; ++j)
                A[1 * N + (100 + j)] = A[0 * N + j];
        }

        // Set diagonal
        for (int i = 0; i < N; ++i)
            A[i * N + i] = drand48();

        // Fill vector b
        for (int i = 0; i < N; ++i)
            b[i] = drand48();

        // Serial result for correctness check
        matrixVectorMult(A, b, x_serial, N);
    }

    double t1 = MPI_Wtime();

    // --- Step 1: Send full vector b to every process ---
    // MPI_Bcast sends from root (rank 0) to everyone.
    // After this call, every process has the full b array.
    MPI_Bcast(b, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // --- Step 2: Scatter rows of A to each process ---
    // MPI_Scatterv is like MPI_Scatter but allows unequal chunk sizes.
    // Process 0 sends sendcounts[i] doubles starting at displs[i] to process i.
    // Each process receives into local_A.
    MPI_Scatterv(A, sendcounts, displs, MPI_DOUBLE,
                 local_A, my_rows * N, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    // --- Step 3: Each process computes its part of x ---
    for (int i = 0; i < my_rows; i++) {
        local_x[i] = 0.0;
        for (int j = 0; j < N; j++)
            local_x[i] += local_A[i * N + j] * b[j];
    }

    // --- Step 4: Gather partial results back to process 0 ---
    // We reuse sendcounts/displs but now they refer to rows (one double per row).
    int* recvcounts = malloc(nprocs * sizeof(int));
    int* rdispls    = malloc(nprocs * sizeof(int));
    int roffset = 0;
    for (int i = 0; i < nprocs; i++) {
        recvcounts[i] = base_rows + (i < remainder ? 1 : 0);
        rdispls[i]    = roffset;
        roffset      += recvcounts[i];
    }

    // MPI_Gatherv collects local_x from each process into x_parallel on root.
    MPI_Gatherv(local_x, my_rows, MPI_DOUBLE,
                x_parallel, recvcounts, rdispls, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    double t2 = MPI_Wtime();

    // --- Correctness check and timing (only rank 0 has full data) ---
    if (rank == 0) {
        double max_error = 0.0;
        for (int i = 0; i < N; ++i) {
            double diff = fabs(x_parallel[i] - x_serial[i]);
            if (diff > max_error) max_error = diff;
        }
        printf("Max error (parallel vs serial): %e\n", max_error);
        printf("Time: %f seconds\n", t2 - t1);

        free(A);
        free(x_serial);
    }

    free(b);
    free(x_parallel);
    free(local_A);
    free(local_x);
    free(sendcounts);
    free(displs);
    free(recvcounts);
    free(rdispls);

    MPI_Finalize();
    return 0;
}
