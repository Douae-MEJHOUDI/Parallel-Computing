#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //printf("hello world \n");
    printf("I am process %d among %d\n", rank, size);
    if (rank == 0){
        printf("I'm here alone\n");
    }

    MPI_Finalize();
    return 0;
}