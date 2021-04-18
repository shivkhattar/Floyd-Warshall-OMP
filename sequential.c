// Import header files for program
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("usage: ./sequential file_name\n");
        printf("file_name: the name of the file containing input node values\n");
        exit(1);
    }

    int rows, columns, middle;
  
    FILE *inputFile;

    int N=0;

    if ((inputFile = fopen(argv[1],"r")) == NULL){
       printf("Error! opening file");
       exit(1);
    }
    fscanf(inputFile, "%d", &N);

    //Define matrix of size N * N to store distances between nodes
    //Initialize all distances to zero
    int distanceMatrix[N][N];
    memset( distanceMatrix, 0, N*N*sizeof(int));    
    
    //Initialize the graph with random distances from the input file
    for (rows = 0; rows < N; rows++)
    {
        for (columns = 0; columns < N; columns++)
        {
            //Distances are generated to be between 0 and 19
            distanceMatrix[rows][columns] = fscanf(inputFile, "%d ", &distanceMatrix[rows][columns]);
        }
    }
    //Define time variable to record start time for execution of program
    double startTime = omp_get_wtime();
  
    // Floyd Warshall Algorithm to converge to minimum distances between all pairs of nodes
    for (middle = 0; middle < N; middle++) {
        int * dm=distanceMatrix[middle];
        for (rows = 0; rows < N; rows++) {
            int * ds=distanceMatrix[rows];
            for (columns = 0; columns < N; columns++) {
                if(ds[columns]>(ds[middle]+dm[columns])) {
                    ds[columns] = ds[middle]+dm[columns];
                }
            }
        }
    }
  
    double time = omp_get_wtime() - startTime;
    printf("Total time for sequential (in sec):%.2f\n", time);
}