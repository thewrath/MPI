#include <iostream>
#include <mpi.h>

#define max_rows 100000
#define send_data_tag 2001
#define return_data_tag 2002


using namespace std;

int main(int argc, char** argv){
    
    long int sum, partialSum = 0;
    int size, rank, avg;
    int rootProcess = 0;
    int numRows;
    
    int arrayToSum[max_rows];

    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int arrayBuffer[avg];
    
    
    if(rank == rootProcess) {
         
         /* I must be the root process, so I will query the user
          * to determine how many numbers to sum. */

         cout << "Entrer le nombre de ligne à additioner : " << endl;
         cin >> numRows;
         
         avg = numRows / size;
      
         if(numRows > max_rows) {
            printf("Too many numbers.\n");
            exit(1);
         }
         
          for(int i = 0; i < numRows; i++) {
            arrayToSum[i] = i + 1;
         }
        
        MPI_Barrier(MPI_COMM_WORLD);
        
    }
    else{
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    MPI_Bcast(&avg, 1, MPI_INT, rootProcess, MPI_COMM_WORLD);
    
    MPI_Scatter(&arrayToSum,avg, MPI_INT,&arrayBuffer, avg, MPI_INT, rootProcess, MPI_COMM_WORLD);

    cout << "I'm rank :" << rank << endl;
    
    for(int i=0; i<avg; i++){
        cout << arrayBuffer[i];
        partialSum += arrayBuffer[i];
    }
    cout << endl;
     
    MPI_Reduce(&partialSum, &sum, 1, MPI_INT, MPI_SUM, rootProcess, MPI_COMM_WORLD);
    
    if(rank == rootProcess){
        cout << "Result = " << sum << endl;
        
    }
    MPI_Finalize();
    return 0;
    
}