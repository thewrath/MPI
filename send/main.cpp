#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	MPI_Status status;
	int size, rank, buffer;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	cout << "My rank is " << rank << " of " << size << endl;

	if(rank == 0){
		buffer = 1000;
	}

	else if(rank == 1){
	 	buffer = 2000;
	}

	if(rank == 0){
		MPI_Send(&buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}

	else if(rank == 1){
		MPI_Recv(&buffer, 1, MPI_INT, 0,0,MPI_COMM_WORLD,&status);
	}

	cout << buffer << endl;

	MPI_Finalize();

	return 0;
}