#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, value;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int v;
    
    do {
    if (rank == 0) scanf("%d", &v);

    MPI_Bcast(&v,1,MPI_INT,0,MPI_COMM_WORLD);
    printf("I, process %d, received %d of process 2\n", rank,v);
    }while(v>=0);
    MPI_Finalize();
}