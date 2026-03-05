#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size, value;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        scanf("%d",&value);
        MPI_Send(&value, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);

    } else if (rank == size - 1) {
        MPI_Recv(&value, 1, MPI_INT, rak -1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value += rank;
        printf("%d\n", value);


    } else {
        MPI_Recv(&value, 1, MPI_INT, rak -1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value += rank;
        printf("%d\n", value);
        MPI_Send(&value, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return 0;
}
