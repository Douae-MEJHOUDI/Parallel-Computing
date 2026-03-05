#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[]){

    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
        int a[4][5]; 

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 5; j++)
                a[i][j] = i * 5 + j + 1;

        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 5; j++)
                printf("%d ", a[i][j]);
            printf("\n");
        }


        MPI_Datatype col_type;
        MPI_Type_vector(4, 1, 5, MPI_INT, &col_type);
        MPI_Type_commit(&col_type);

        for (int col=0; col<5; col++){
            MPI_Send(&a[0][col], 1, col_type, 1, 0, MPI_COMM_WORLD);
        }


    }
    if (rank == 1){
        int b[5][4];
        for (int col=0; col<5; col++){
            MPI_Recv(&b[col][0], 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        for (int i = 0; i < 5; i++){
            for (int j = 0; j < 4; j++)
                printf("%d ", b[i][j]);
            printf("\n");
        }
    }
    MPI_Finalize();


    return 0;
}