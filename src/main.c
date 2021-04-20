#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <stdbool.h>
#include <getopt.h>

#include "generate.h"
#include "sequential_fw.h"
#include "parallel_fw.h"
#include "validator.h"

void getOptions(int argc, char **argv, int *nodeCount, double *probability, int *blockSize, int *minNumThreads,
                int *maxNumThreads, bool *save, bool *prin, bool *validate);

int *runSequentialFloydWarshall(const int *distanceMatrix, const int nodeCount, const bool printOutput,
                                const bool saveOutput);

int *
runParallelFloydWarshall(const int *distanceMatrix, const int nodeCount, const int blockSize, const int threadCount);

int main(int argc, char **argv) {
    int minNumThreads = 1, maxNumThreads = 1, blockSize = 32, nodeCount = 1024;
    double probability = 0.5;
    bool save = false, print = false, validate = false;
    getOptions(argc, argv, &nodeCount, &probability, &blockSize, &minNumThreads, &maxNumThreads, &save, &print,
               &validate);
    if (nodeCount < blockSize) {
        printf("Incorrect block size: %d. Block size should be less than node count: %d\n", blockSize, nodeCount);
        exit(1);
    } else if (nodeCount % blockSize != 0) {
        printf("Incorrect block size: %d. Blocksize should exactly divide node count: %d\n", blockSize, nodeCount);
        exit(1);
    }
    int *distanceMatrix = generateRandomWeightedEdges(nodeCount, probability, print, save);

    double startTime = omp_get_wtime();
    int *seqOutput = runSequentialFloydWarshall(distanceMatrix, nodeCount, print, save);
    double seqTime = omp_get_wtime() - startTime;
    printf("Total time for sequential (in sec):%.2f\n", seqTime);

    for (int threadCount = minNumThreads; threadCount <= maxNumThreads; threadCount++) {
        startTime = omp_get_wtime();
        int *parallelOutput = runParallelFloydWarshall(distanceMatrix, nodeCount, blockSize, threadCount);
        double parallelTime = omp_get_wtime() - startTime;
        printf("\nTotal time for %d threads (in sec):%.2f\n", threadCount, parallelTime);
        printf("Speedup for %d threads: %f\n", threadCount, seqTime / parallelTime);
        if (validate) validateOutputs(parallelOutput, seqOutput, nodeCount);
    }
}

void getOptions(int argc, char **argv, int *nodeCount, double *probability, int *blockSize, int *minNumThreads,
                int *maxNumThreads, bool *save, bool *print, bool *validate) {
    int opt = -1;
    while ((opt = getopt(argc, argv, "n:e:b:t:m:spv")) != -1) {
        switch (opt) {
            case 'n':
                sscanf(optarg, "%d", nodeCount);
                break;
            case 'e':
                sscanf(optarg, "%lf", probability);
                break;
            case 'b':
                sscanf(optarg, "%d", blockSize);
                break;
            case 't':
                sscanf(optarg, "%d", maxNumThreads);
                break;
            case 'm':
                sscanf(optarg, "%d", minNumThreads);
            case 's' :
                *save = true;
                break;
            case 'p':
                *print = true;
                break;
            case 'v':
                *validate = true;
                break;
            case '?':
                if (optopt == 'c')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                exit(1);
            default:
                abort();
        }
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
runParallelFloydWarshall(const int *distanceMatrix, const int nodeCount, const int blockSize, const int threadCount) {
    int *output;
    output = malloc(nodeCount * nodeCount * sizeof(int));
    memset(output, 0, nodeCount * nodeCount * sizeof(int));
    parallelFloydWarshall(distanceMatrix, output, blockSize, nodeCount, threadCount, true, true);
    return output;
}