#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_FEATURES  5
#define N_SAMPLES   100
#define MAX_EPOCHS  1000
#define LR          0.1
#define THRESHOLD   1e-4

typedef struct {
    double x[N_FEATURES];
    double y;
} Sample;

void generate_data(Sample *data, int n) {
    double w_true[N_FEATURES] = {2.0, -1.0, 0.5, 0.3, -0.7};
    srand(42);
    for (int i = 0; i < n; i++) {
        double y = 0.0;
        for (int k = 0; k < N_FEATURES; k++) {
            data[i].x[k] = (double)rand() / RAND_MAX;
            y += w_true[k] * data[i].x[k];
        }
        data[i].y = y + 0.05 * ((double)rand() / RAND_MAX - 0.5);
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // create MPI type for Sample struct
    Sample s;
    MPI_Aint base, addr_x, addr_y;
    MPI_Get_address(&s,   &base);
    MPI_Get_address(&s.x, &addr_x);
    MPI_Get_address(&s.y, &addr_y);
    int          block_sizes[2]  = {N_FEATURES, 1};
    MPI_Aint     offsets[2]      = {addr_x - base, addr_y - base};
    MPI_Datatype field_types[2]  = {MPI_DOUBLE, MPI_DOUBLE};
    MPI_Datatype sample_type;
    MPI_Type_create_struct(2, block_sizes, offsets, field_types, &sample_type);
    MPI_Type_commit(&sample_type);

    // each process figures out how many samples it gets
    int local_n = N_SAMPLES / nprocs + (rank < N_SAMPLES % nprocs ? 1 : 0);

    // root generates all data and prepares scatter info
    Sample *all_data = NULL;
    int *counts = NULL, *starts = NULL;
    if (rank == 0) {
        all_data = malloc(N_SAMPLES * sizeof(Sample));
        counts   = malloc(nprocs * sizeof(int));
        starts   = malloc(nprocs * sizeof(int));
        generate_data(all_data, N_SAMPLES);
        int offset = 0;
        for (int i = 0; i < nprocs; i++) {
            counts[i] = N_SAMPLES / nprocs + (i < N_SAMPLES % nprocs ? 1 : 0);
            starts[i] = offset;
            offset += counts[i];
        }
    }

    Sample *my_data = malloc(local_n * sizeof(Sample));
    MPI_Scatterv(all_data, counts, starts, sample_type,
                 my_data, local_n, sample_type, 0, MPI_COMM_WORLD);

    // gradient descent
    double w[N_FEATURES] = {0};
    double t0 = MPI_Wtime();

    for (int epoch = 1; epoch <= MAX_EPOCHS; epoch++) {
        double grad[N_FEATURES] = {0}, loss = 0;
        for (int i = 0; i < local_n; i++) {
            double pred = 0;
            for (int k = 0; k < N_FEATURES; k++) pred += w[k] * my_data[i].x[k];
            double err = pred - my_data[i].y;
            loss += err * err;
            for (int k = 0; k < N_FEATURES; k++) grad[k] += 2 * err * my_data[i].x[k];
        }

        MPI_Allreduce(MPI_IN_PLACE, grad, N_FEATURES, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(MPI_IN_PLACE, &loss, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        double mse = loss / N_SAMPLES;
        for (int k = 0; k < N_FEATURES; k++) w[k] -= LR * grad[k] / N_SAMPLES;

        if (rank == 0 && epoch % 10 == 0)
            printf("Epoch %3d | MSE=%.6f | w[0]=%.4f w[1]=%.4f\n", epoch, mse, w[0], w[1]);
        if (mse < THRESHOLD) { if (rank == 0) printf("Converged at epoch %d\n", epoch); break; }
    }

    if (rank == 0) printf("Time: %.3f s\n", MPI_Wtime() - t0);

    free(my_data);
    if (rank == 0) { free(all_data); free(counts); free(starts); }
    MPI_Type_free(&sample_type);
    MPI_Finalize();
    return 0;
}
