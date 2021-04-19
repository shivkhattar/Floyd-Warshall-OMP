#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

static inline void
floydWarshallPerBlock(int *output, const int *startBlock, const int *endBlock, const int blockSize,
                      const int nodeCount) {
    for (int middle = 0; middle < blockSize; middle++) {
        for (int start = 0; start < blockSize; start++) {
            for (int end = 0; end < blockSize; end++) {
                int sum = startBlock[start * nodeCount + middle] + endBlock[middle * nodeCount + end];
                if (sum <= output[start * nodeCount + end]) {
                    output[start * nodeCount + end] = sum;
                }
            }
        }
    }
}

void parallelFloydWarshall(const int *distanceMatrix, int *output, const int blockSize, const int nodeCount,
                           const int nthreads) {
    memcpy(output, distanceMatrix, nodeCount * nodeCount * sizeof(int));
    const int blocks = nodeCount / blockSize;
    omp_set_num_threads(nthreads);
    for (int middleBlock = 0; middleBlock < blocks; middleBlock++) {
        floydWarshallPerBlock(&output[middleBlock * blockSize * nodeCount + middleBlock * blockSize],
                              &output[middleBlock * blockSize * nodeCount + middleBlock * blockSize],
                              &output[middleBlock * blockSize * nodeCount + middleBlock * blockSize], blockSize,
                              nodeCount);

#pragma omp parallel for schedule(dynamic)
        for (int endBlock = 0; endBlock < blocks; endBlock++) {
            if (endBlock == middleBlock) continue;
            floydWarshallPerBlock(&output[middleBlock * blockSize * nodeCount + endBlock * blockSize],
                                  &output[middleBlock * blockSize * nodeCount + middleBlock * blockSize],
                                  &output[middleBlock * blockSize * nodeCount + endBlock * blockSize], blockSize,
                                  nodeCount);

        }

#pragma omp parallel for schedule(dynamic)
        for (int startBlock = 0; startBlock < blocks; startBlock++) {
            if (startBlock == middleBlock) continue;
            floydWarshallPerBlock(&output[startBlock * blockSize * nodeCount + middleBlock * blockSize],
                                  &output[startBlock * blockSize * nodeCount + middleBlock * blockSize],
                                  &output[middleBlock * blockSize * nodeCount + middleBlock * blockSize], blockSize,
                                  nodeCount);

            for (int endBlock = 0; endBlock < blocks; endBlock++) {
                if (endBlock == middleBlock) continue;
                floydWarshallPerBlock(&output[startBlock * blockSize * nodeCount + endBlock * blockSize],
                                      &output[startBlock * blockSize * nodeCount + middleBlock * blockSize],
                                      &output[middleBlock * blockSize * nodeCount + endBlock * blockSize], blockSize,
                                      nodeCount);
            }
        }
    }
}