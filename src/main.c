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

int main(int argc, char **argv) {
    int minNumThreads = 1, maxNumThreads = 1, blockSize = 32, nodeCount = 1024;
    double probability = 0.5;
    bool save = false, print = false, validate = false;
    getOptions(argc, argv, &nodeCount, &probability, &blockSize, &minNumThreads, &maxNumThreads, &save, &print,
               &validate);
    if (nodeCount < blockSize) blockSize = nodeCount;
    int *distanceMatrix = generateRandomWeightedEdges(nodeCount, probability, print, save);

    int *seqOutput;
    seqOutput = malloc(nodeCount * nodeCount * sizeof(int));
    memset(seqOutput, 0, nodeCount * nodeCount * sizeof(int));

    double startTime = omp_get_wtime();
    sequential_floyd_warshall(distanceMatrix, seqOutput, nodeCount);
    double seqTime = omp_get_wtime() - startTime;
    printf("Total time for sequential (in sec):%.2f\n", seqTime);

    int *output;
    output = malloc(nodeCount * nodeCount * sizeof(int));

    for (int threadCount = minNumThreads; threadCount <= maxNumThreads; threadCount++) {
        memset(output, 0, nodeCount * nodeCount * sizeof(int));
        startTime = omp_get_wtime();
        parallel_floyd_warshall(distanceMatrix, output, blockSize, nodeCount, threadCount);
        double parallelTime = omp_get_wtime() - startTime;
        printf("\nTotal time for %d threads (in sec):%.2f\n", threadCount, parallelTime);
        printf("Speedup for %d threads: %f\n", threadCount, seqTime / parallelTime);
    }

    if (validate) validateOutputs(output, seqOutput, nodeCount);
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