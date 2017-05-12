#include <mpi.h>
#include <iostream>

#define VECTOR_SIZE 30
#define send_data_tag 2001
#define return_data_tag 2002

using namespace std;

int vectorA[VECTOR_SIZE];
int vectorB[VECTOR_SIZE];

void vectorInit();

int main(int argc, char** argv){

	int rank;
	int size;

	MPI_Status status;

	int buffer;

	int root_process = 0;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//le process root 
	if(rank == root_process){

		int arrayToGive[VECTOR_SIZE];
		int avgRowByProcess = VECTOR_SIZE/size;
		int offset = avgRowByProcess;

		vectorInit();
		for(int id=root_process+1; id<size; id++){
			for(int j=0+(avgRowByProcess*id); j<avgRowByProcess*id + avgRowByProcess; j++){
				buffer = vectorA[j];
				MPI_Send(&buffer, 1, MPI_INT, id, send_data_tag, MPI_COMM_WORLD);
			}
		}
		cout << "all is send" << endl;
	}
	else{
		int vectorLocal[VECTOR_SIZE];
		for(int i=0; i<VECTOR_SIZE; i++){
			MPI_Recv(&buffer, 1, MPI_INT, root_process, send_data_tag,MPI_COMM_WORLD, &status);
			vectorLocal[i] = buffer;	
			cout << buffer;	
		}
		cout << endl << "all is receive";
	}
	

	MPI_Finalize();
	return 0;
}

void vectorInit(){
	for(int i=0; i<VECTOR_SIZE; i++){
		vectorA[i] = i;
		vectorB[i] = i;
		cout << vectorA[i];
	}
	cout << endl;
}