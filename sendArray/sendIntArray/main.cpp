#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv)
{
	int size, rank;

	const int sender = 0;
	const int receiver = 1;

	const int stride = 1;
	const int tag = 1;

	const int arraySize = 20;

	//MPI init
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if(size < 2){
		cout << "Require at least to processor" << endl;
		exit(-1);
	}

	if(rank == sender){
		int array[arraySize];
		int arrayToSend[arraySize/2];
		
		//fill the first array 
		for(int i=0; i<arraySize; i++){
			array[i] = i;
			cout << i << " ";
		}
		cout << endl;
		
		//fill the array wich will traits by the seconde core 
		for(int i=arraySize/2; i<arraySize; i++){
			arrayToSend[i] = array[i];
		}

		MPI_Datatype arrayType;

		MPI_Type_vector(arraySize, 1, stride, MPI_INT, &arrayType);
		MPI_Type_commit(&arrayType);

		MPI_Send(arrayToSend, 1, arrayType, receiver, tag, MPI_COMM_WORLD);

		MPI_Type_free(&arrayType);

	}
	else if(rank == receiver){
		int localArray[arraySize/2];

		MPI_Status status;

		MPI_Recv(localArray, arraySize, MPI_INT, sender, tag, MPI_COMM_WORLD, &status);

		cout << "Array of second process" << endl;

		for(int i=0; i<arraySize/2; i++){
			cout << localArray[i] << " ";
		}

		cout << endl;

	}

	MPI_Finalize();
	return 0;
}