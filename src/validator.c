#include <stdio.h>
#include <stdlib.h>

void validateOutputs(const int *parallelOutput, const int *seqOutput, const int nodeCount) {
    for (int i = 0; i < nodeCount; i++) {
        for (int j = 0; j < nodeCount; j++) {
            if (parallelOutput[i * nodeCount + j] != seqOutput[i * nodeCount + j]) {
                printf("\nOutput did not match at [%d][%d]: %d vs sequential's %d!\n", i, j,
                       parallelOutput[i * nodeCount + j],
                       seqOutput[i * nodeCount + j]);
                exit(1);
            }
        }
    }
    printf("Outputs Match\n");
}