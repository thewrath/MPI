#include <iostream>
#include <mpi.h>

#define send_data_tag 2001
#define return_data_tag 2002

using namespace std;

int main(int argc, char **argv)
{
	int size, rank, numRowSend, numRowReceive;

	int rootProcess = 0;

	MPI_Status status;

	const int vectorSize = 20; 
	int vector1[vectorSize];
	int vector2[vectorSize];

	int vectorTmp1[vectorSize];
	int vectorTmp2[vectorSize];

	int vectorResult[vectorSize];
	int vectorResultPartial[vectorSize];
	int vectorResultTmp[vectorSize];

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	cout << "Processs number : " << rank << endl;

	if(rank == rootProcess){
		int startRow, endRow, avgRow;
		//vectors initialisation
		for(int i=0; i<vectorSize; i++){
			vector1[i] = i;
			vector2[i] = i + 3;
			cout << " vector1 :" << vector1[i] << " ";
			cout << " vector2 :" << vector2[i] << " " << endl;
		} 

		avgRow = vectorSize/size;


		for(int i=1; i<size; i++){
			startRow = i*avgRow;
			endRow = (i+1)*avgRow-1;

			numRowSend = endRow-startRow+1;
			MPI_Send(&numRowSend, 1, MPI_INT, i, send_data_tag, MPI_COMM_WORLD);
			MPI_Send(&vector1[startRow], numRowSend,MPI_INT, i, send_data_tag, MPI_COMM_WORLD);
			MPI_Send(&vector2[startRow], numRowSend,MPI_INT, i, send_data_tag, MPI_COMM_WORLD);
		}

		for(int i=0; i<avgRow; i++){
			vectorResult[i] = vector1[i]*vector2[i];
		}

		for(int i=1; i<size; i++){
			startRow = i*avgRow;
			endRow = (i+1)*avgRow-1;

			MPI_Recv(vectorResultPartial, numRowSend, MPI_INT, i, return_data_tag, MPI_COMM_WORLD, &status);
			for(int k=0; k<numRowSend; k++){
				cout << "vector result partial : " << i << " " << vectorResultPartial[k] << " " << endl;
			}
			for(int j=startRow; j<startRow+numRowSend; j++){
				vectorResult[j]  = vectorResultPartial[j-startRow];
			}
			
		}

		cout << "Result :" << endl;

		for(int i = 0; i<vectorSize; i++){
			cout << "vector result : " << vectorResult[i] << " " << endl;
		}

		cout << endl;

	}

	else{

		MPI_Recv(&numRowReceive, 1, MPI_INT, rootProcess, send_data_tag, MPI_COMM_WORLD, &status);
		MPI_Recv(vectorTmp1, numRowReceive, MPI_INT, rootProcess, send_data_tag, MPI_COMM_WORLD, &status);
		MPI_Recv(vectorTmp2, numRowReceive, MPI_INT, rootProcess, send_data_tag, MPI_COMM_WORLD, &status);
		
		for(int i=0; i<numRowReceive; i++){
			cout << "process : " << rank << " vectortmp1 : "<< vectorTmp1[i] << " " << endl;
		}

		for(int i=0; i<numRowReceive; i++){
			cout << "process : " << rank <<" vectortmp2 : "<< vectorTmp2[i] << " " << endl;
		}

		for(int i=0; i<numRowReceive; i++){
			vectorResultTmp[i] = vectorTmp1[i]*vectorTmp2[i];
		}

		MPI_Send(&vectorResultTmp[0], numRowReceive, MPI_INT, rootProcess, return_data_tag, MPI_COMM_WORLD);
		cout << endl;
	}



	MPI_Finalize();

	return 0;
}