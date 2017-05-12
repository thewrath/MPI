#include <iostream>
#include <math.h>
#include <mpi.h>

using namespace std;

bool testDivide(int num,int denom);

int main(int argc, char** argv){
    
    const int MAX_NUMBER = 10000;
    
    int size, rank, number, avg;
    
    int arrayFromOne[MAX_NUMBER];
    int arrayFromOneBuffer[avg];
    
    int rootProcess = 0;
    
    int result = 0;
    
    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == rootProcess){
        cout << "Entrer le nombre à tester :" << endl;
        cin >> number;
        
        if(number > MAX_NUMBER){
            cout << "Trop grand" << endl;
            exit(1);
        }
        //on test seulement de 2 à number -1
        for(int i=2; i<number; i++){
            arrayFromOne[i-2] = i;
        }
        
        avg = number/size;
    }
   
    MPI_Barrier(MPI_COMM_WORLD);
    
    MPI_Bcast(&avg, 1, MPI_INT, rootProcess, MPI_COMM_WORLD);
    MPI_Bcast(&number, 1, MPI_INT,  rootProcess, MPI_COMM_WORLD);
    MPI_Scatter(&arrayFromOne, avg, MPI_INT, &arrayFromOneBuffer, avg, MPI_INT, rootProcess, MPI_COMM_WORLD);
    
    if(rank == rootProcess){
        
        cout << "I'm rank :" << rank << endl;
    
        for(int i=0; i<avg; i++){
            if(arrayFromOneBuffer[i] !=0 ){
                
                cout << arrayFromOneBuffer[i];
                if(testDivide(number, arrayFromOneBuffer[i])){
                    cout << " ici :::::: " << i ;
                    result +=1;
                    break;
                }
            }
        }
        
        cout << endl;
        
        if(result > 0 ){
            cout << number << " is not a prime number " << endl;
        }
        else{
            cout << number << " is a prime number " << endl;
        }
        
    }
    else{
        
        int partialResult = 0;
        cout << "I'm rank :" << rank << endl;
    
        for(int i=0; i<avg; i++){
            if(arrayFromOneBuffer[i] !=0 ){
                
                cout << arrayFromOneBuffer[i];
                if(testDivide(number, arrayFromOneBuffer[i])){
                    partialResult = 1;
                    
                    cout << " ici :::::: " << i ;
                    
                    break;
                }
            }
        }
        MPI_Reduce(&partialResult, &result, 1, MPI_INT, MPI_SUM, rootProcess, MPI_COMM_WORLD);
        cout << endl;
    }
    MPI_Finalize();
    
    return 0;
}

bool testDivide(int num, int denom){
    if(num%denom == 0)
        return true;
    return false;
}