#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    int size, rank;
    const int bigsize=50;
    const int stride = 5;
    const int count = (bigsize + stride - 1)/stride;

    const int sender = 0;
    const int receiver = 1;
    const int mytag = 1;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (size < 2) {
        fprintf(stderr,"%s: Require at least two processors.\n", argv[0]);
        MPI_Finalize();
        exit(-1);
    }


    if(rank == sender)
    {
        double bigarray[bigsize];
        for (int i=0; i<bigsize; i++)
            bigarray[i] = 0.;

        for (int i=0; i<bigsize; i+=stride)
            bigarray[i] = i/stride;

        printf("[%d]: ", rank);
        for (int i=0; i<bigsize; i++)
            printf("%lf ", bigarray[i]);
        printf("\n");

        MPI_Datatype everyfifth;

        MPI_Type_vector( count, 1, stride, MPI_DOUBLE, &everyfifth);
        MPI_Type_commit(&everyfifth);

        MPI_Send(bigarray, 1, everyfifth, receiver, mytag, MPI_COMM_WORLD);

        MPI_Type_free(&everyfifth);
    }
    else if( rank == receiver )
    {
        double littlearray[count];

        MPI_Status status;

        MPI_Recv(littlearray, count, MPI_DOUBLE, sender, mytag,
                    MPI_COMM_WORLD, &status);

        printf("[%d]: ", rank);
        for (int i=0; i<count; i++)
            printf("%lf ", littlearray[i]);
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}
