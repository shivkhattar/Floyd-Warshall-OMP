#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("usage: ./parallel fileName minNumThreads maxNumThreads flag\n");
        printf("fileName: the name of the file containing input node values\n");
        printf("minNumThreads: minimum number of threads to parallelize\n");
        printf("maxNumThreads: maximum number of threads to parallelize\n");
        printf("flag: (1 or 0) to check if sequential and parallel code match\n");
        exit(1);
    }

    int minNumThreads = atoi(argv[2]);
    int maxNumThreads = atoi(argv[3]);
    int flag = atoi(argv[4]);
    FILE *inputFile, *outputFile;
    int nodeCount = 0;

    if ((inputFile = fopen(argv[1], "r")) == NULL) {
        printf("Error in opening file");
        exit(1);
    }
    fscanf(inputFile, "%d", &nodeCount);

    int *distanceMatrix[nodeCount];
    for (int i = 0; i < nodeCount; i++) {
        distanceMatrix[i] = malloc(nodeCount * sizeof(int));
    }

    //Initialize the graph with random distances from the input file
    for (int i = 0; i < nodeCount; i++) {
        for (int j = 0; j < nodeCount; j++) {
            //Distances are generated to be between 1 and 20
            fscanf(inputFile, "%d", &distanceMatrix[i][j]);
        }
    }
    fclose(inputFile);

    int start, end, middle;

    for (int nthreads = minNumThreads; nthreads <= maxNumThreads; nthreads++) {
        //Define different number of threads
        omp_set_num_threads(nthreads);

        // Define iterator to iterate over distance matrix
        //Define time variable to record start time for execution of program
        double startTime = omp_get_wtime();

        /* Taking a node as mediator
        check if indirect distance between source and distance via mediator
        is less than direct distance between them */
        #pragma omp parallel shared(distanceMatrix)
        // Floyd Warshall Algorithm to converge to minimum distances between all pairs of nodes
        for (middle = 0; middle < nodeCount; middle++) {
            int *dm = distanceMatrix[middle];
        #pragma omp parallel for private(start, end) schedule(dynamic)
            for (start = 0; start < nodeCount; start++) {
                int *ds = distanceMatrix[start];
                for (end = 0; end < nodeCount; end++) {
                    if (ds[middle] + dm[end] < ds[end]) {
                        ds[end] = ds[middle] + dm[end];
                    }
                }
            }
        }

        double time = omp_get_wtime() - startTime;
        printf("Total time for thread %d (in sec):%.2f\n", nthreads, time);
    }

    // Compare results of parallel execution with sequential execution of Floyd Warshall.
    if(flag>0){
        if ((outputFile = fopen(strcat(argv[1], "_out"), "r")) == NULL) {
            printf("Cannot open the output file %s\n", argv[1]);
            exit(1);
        }

        int nodeCountSaved = 0;
        fscanf(outputFile, "%d", &nodeCountSaved);
        if (nodeCount != nodeCountSaved) {
            printf("Sequential and parallel results do not match please verify code.\n");
            exit(1);
        }

        int tempVar = 0;
        for (start = 0; start < nodeCount; start++) {
            for (end = 0; end < nodeCount; end++) {
                fscanf(outputFile, "%d ", &tempVar);
                if (tempVar != distanceMatrix[start][end]) {
                    printf("Sequential and parallel results do not match please verify code 2.\n");
                    exit(1);
                }
            }
        }
        printf("The results of sequential and parallel code match!");
        fclose(outputFile);
    }
    return 0;
}