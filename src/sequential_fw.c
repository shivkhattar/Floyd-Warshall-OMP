#include <string.h>

void sequentialFloydWarshall(const int *distanceMatrix, int *output, const int nodeCount) {
    memcpy(output, distanceMatrix, nodeCount * nodeCount * sizeof(int));
    for (int middle = 0; middle < nodeCount; middle++) {
        for (int start = 0; start < nodeCount; start++) {
            for (int end = 0; end < nodeCount; end++) {
                if (output[start * nodeCount + end] >
                    output[start * nodeCount + middle] + output[middle * nodeCount + end]) {
                    output[start * nodeCount + end] =
                            output[start * nodeCount + middle] + output[middle * nodeCount + end];
                }
            }
        }
    }
}