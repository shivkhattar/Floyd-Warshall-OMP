#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <stdbool.h>

#include "generate.h"
#include "sequential_fw.h"
#include "parallel_fw.h"
#include "validator.h"

int main(int argc, char *argv[]) {
    int maxNumThreads = 10;
    int blockSize = 32;
    int nodeCount = 1024;

    int *distanceMatrix = generateRandomWeightedEdges(nodeCount, 0.5, false, true);

    int *seqOutput;
    seqOutput = malloc(nodeCount * nodeCount * sizeof(int));
    memset(seqOutput, 0, nodeCount * nodeCount * sizeof(int));

    double startTime = omp_get_wtime();
    sequential_floyd_warshall(distanceMatrix, seqOutput, nodeCount);
    double seqTime = omp_get_wtime() - startTime;
    printf("Total time for sequential (in sec):%.2f\n", seqTime);

    int *output;
    output = malloc(nodeCount * nodeCount * sizeof(int));
    memset(output, 0, nodeCount * nodeCount * sizeof(int));
    startTime = omp_get_wtime();
    int nthreads = maxNumThreads;
    parallel_floyd_warshall(distanceMatrix, output, blockSize, nodeCount, nthreads);
    double parallelTime = omp_get_wtime() - startTime;
    printf("Total time for %d threads (in sec):%.2f\n", nthreads, parallelTime);
    validateOutputs(output, seqOutput, nodeCount);
    printf("\nSpeedup: %f\n\n", seqTime / parallelTime);
}






