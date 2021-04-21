#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <stdbool.h>

#include "generate.h"
#include "sequential_fw.h"
#include "parallel_fw.h"
#include "validator.h"
#include "util.h"

int *runSequentialFloydWarshall(const int *distanceMatrix, const int nodeCount, const bool printOutput,
                                const bool saveOutput);

void
runBenchmark(int nodeCount, double probability, int blockSize, int minNumThreads, int maxNumThreads, int maxIterations,
             bool validate);

void run(int nodeCount, double probability, int blockSize, int minNumThreads, int maxNumThreads, bool print, bool save,
         bool validate, int maxIterations);

int *
runParallelFloydWarshall(const int *distanceMatrix, const int nodeCount, const int blockSize, const int threadCount,
                         const bool printOutput, const bool saveOutput);

int main(int argc, char **argv) {
    int minNumThreads = 1, maxNumThreads = 10, blockSize = 32, nodeCount = 1024, maxIterations = 1;
    double probability = 0.5;
    bool save = false, print = false, validate = false, benchmark = false;
    getOptions(argc, argv, &nodeCount, &probability, &blockSize, &minNumThreads, &maxNumThreads, &maxIterations, &save,
               &print, &validate, &benchmark);
    if (nodeCount < blockSize) {
        printf("Incorrect block size: %d. Block size should be less than node count: %d\n", blockSize, nodeCount);
        exit(1);
    } else if (nodeCount % blockSize != 0) {
        printf("Incorrect block size: %d. Blocksize should exactly divide node count: %d\n", blockSize, nodeCount);
        exit(1);
    }
    printf("Nodes\t Prob  \tBlock\tThread\t  Seq\tPara \tSpeedup\n");
    if (benchmark)
        runBenchmark(nodeCount, probability, blockSize, minNumThreads, maxNumThreads, maxIterations, validate);
    else
        run(nodeCount, probability, blockSize, minNumThreads, maxNumThreads, print, save, validate, maxIterations);
}

void
runBenchmark(int nodeCount, double probability, int blockSize, int minNumThreads, int maxNumThreads, int maxIterations,
             bool validate) {
    for (; probability <= 1; probability += 0.5) {
        for (; nodeCount <= 2048; nodeCount *= 2) {
            for (; blockSize <= nodeCount / 2 && blockSize <= 128; blockSize *= 2) {
                run(nodeCount, probability, blockSize, minNumThreads, maxNumThreads, false, false, validate,
                    maxIterations);
            }
        }
    }
}

void
run(int nodeCount, double probability, int blockSize, int minNumThreads, int maxNumThreads, bool print, bool save,
    bool validate, int maxIterations) {
    int *distanceMatrix = generateRandomWeightedEdges(nodeCount, probability, print, save);
    double startTime = 0;
    int *seqOutput;
    double seqTimes[maxIterations];
    for (int iteration = 0; iteration < maxIterations; iteration++) {
        startTime = omp_get_wtime();
        seqOutput = runSequentialFloydWarshall(distanceMatrix, nodeCount, print, save);
        seqTimes[iteration] = omp_get_wtime() - startTime;
    }
    double seqTime = getAverageTime(seqTimes, maxIterations);

    for (int threadCount = minNumThreads; threadCount <= maxNumThreads; threadCount++) {
        int *parallelOutput;
        double parallelTimes[maxIterations];
        for (int iteration = 0; iteration < maxIterations; iteration++) {
            startTime = omp_get_wtime();
            parallelOutput = runParallelFloydWarshall(distanceMatrix, nodeCount, blockSize, threadCount, print,
                                                      save);
            parallelTimes[iteration] = omp_get_wtime() - startTime;
        }
        double parallelTime = getAverageTime(parallelTimes, maxIterations);
        printf("%5d\t%5.2f\t%5d\t", nodeCount, probability, blockSize);
        printf("%5d\t%5.2f\t%5.2f\t%5.2fx\n", threadCount, seqTime, parallelTime, seqTime / parallelTime);
        if (validate) validateOutputs(parallelOutput, seqOutput, nodeCount);
    }
}

int *runSequentialFloydWarshall(const int *distanceMatrix, const int nodeCount, const bool printOutput,
                                const bool saveOutput) {
    int *seqOutput;
    seqOutput = malloc(nodeCount * nodeCount * sizeof(int));
    memset(seqOutput, 0, nodeCount * nodeCount * sizeof(int));
    sequentialFloydWarshall(distanceMatrix, seqOutput, nodeCount, printOutput, saveOutput);
    return seqOutput;
}

int *
runParallelFloydWarshall(const int *distanceMatrix, const int nodeCount, const int blockSize, const int threadCount,
                         const bool printOutput, const bool saveOutput) {
    int *output;
    output = malloc(nodeCount * nodeCount * sizeof(int));
    memset(output, 0, nodeCount * nodeCount * sizeof(int));
    parallelFloydWarshall(distanceMatrix, output, blockSize, nodeCount, threadCount, printOutput, saveOutput);
    return output;
}