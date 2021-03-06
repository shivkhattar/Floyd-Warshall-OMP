#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

#include "util.h"

#define MAX_DISTANCE 100

int *generateRandomWeightedEdges(const int nodeCount, const double edgeProbability, const bool printMatrix,
                                 const bool saveMatrix) {
    if (nodeCount < 0) {
        printf("Invalid number of nodes provided: %d\n", nodeCount);
        exit(1);
    }
    if (edgeProbability < 0 || edgeProbability > 1) {
        printf("Invalid edge probability provided: %d\n", nodeCount);
        exit(1);
    }
    srand((unsigned) time(0));
    int *matrix;
    matrix = malloc(nodeCount * nodeCount * sizeof(int));
    memset(matrix, 0, nodeCount * nodeCount * sizeof(int));
    for (int i = 0; i < nodeCount; i++) {
        for (int j = 0; j < nodeCount; j++) {
            int distance = 0;
            if (i != j) {
                double randomProbability = (double) rand() / (double) RAND_MAX;
                if (randomProbability <= edgeProbability) {
                    distance = 1 + (rand() % MAX_DISTANCE);
                } else {
                    distance = INT_MAX / 2;
                }
            }
            matrix[i * nodeCount + j] = distance;
        }
    }
    if (printMatrix) {
        printf("Printing generated Matrix: \n");
        print(matrix, nodeCount);
    }
    if (saveMatrix) save(matrix, nodeCount, "data/matrix");
    return matrix;
}
