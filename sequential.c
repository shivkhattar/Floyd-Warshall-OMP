#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: ./sequential file_name\n");
        printf("file_name: the name of the file containing input node values\n");
        exit(1);
    }

    FILE *inputFile, *outputFile;
    int nodeCount = 0;

    if ((inputFile = fopen(argv[1], "r")) == NULL) {
        printf("Error in opening file");
        exit(1);
    }
    fscanf(inputFile, "%d", &nodeCount);

    int distanceMatrix[nodeCount][nodeCount];
    memset(distanceMatrix, 0, nodeCount * nodeCount * sizeof(int));

    //Initialize the graph with random distances from the input file
    for (int i = 0; i < nodeCount; i++) {
        for (int j = 0; j < nodeCount; j++) {
            //Distances are generated to be between 1 and 20
            fscanf(inputFile, "%d", &distanceMatrix[i][j]);
        }
    }
    fclose(inputFile);

    int start, end, middle;
    //Define time variable to record start time for execution of program
    double startTime = omp_get_wtime();

    // Floyd Warshall Algorithm to converge to minimum distances between all pairs of nodes
    for (middle = 0; middle < nodeCount; middle++) {
        int *dm = distanceMatrix[middle];
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
    printf("Total time for sequential (in sec):%.2f\n", time);

    // Write matrix with minimum distances to outputFile
    if ((outputFile = fopen(strcat(argv[1], "_out"), "w+t")) == NULL) {
        printf("Cannot create the file %s\n", argv[1]);
        exit(1);
    }
    fprintf(outputFile, "%d\n", nodeCount);
    for (start = 0; start < nodeCount; start++) {
        for (end = 0; end < nodeCount; end++) {
            fprintf(outputFile, "%d ", distanceMatrix[start][end]);
        }
        fprintf(outputFile, "\n");
    }
    fclose(outputFile);
}