#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DISTANCE 100

int *generateRandomWeightedEdges(const int nodeCount) {
    if (nodeCount < 0) {
        printf("Invalid number of nodes provided: %d", nodeCount);
        exit(1);
    }

    int *matrix;
    matrix = malloc(nodeCount * nodeCount * sizeof(int));
    memset(matrix, 0, nodeCount * nodeCount * sizeof(int));
    for (int i = 0; i < nodeCount; i++) {
        for (int j = 0; j < nodeCount; j++) {
            int distance = 0;
            if (i != j) distance = 1 + (rand() % MAX_DISTANCE);
            matrix[i * nodeCount + j] = distance;
        }
    }

    printf("Generated %d graph nodes\n", nodeCount);
//    for (int i = 0; i < nodeCount; i++) {
//        for (int j = 0; j < nodeCount; j++) {
//            printf("%d ", matrix[i * nodeCount + j]);
//        }
//        printf("\n");
//    }
    return matrix;
}
