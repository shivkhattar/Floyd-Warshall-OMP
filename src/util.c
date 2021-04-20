#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(const int *output, const int nodeCount) {
    for (int i = 0; i < nodeCount; i++) {
        for (int j = 0; j < nodeCount; j++) {
            printf("%d ", output[i * nodeCount + j]);
        }
        printf("\n");
    }
}

void save(const int *output, const int nodeCount, const char *fileName) {
    printf("Saving: %s\n", fileName);
    FILE *fp;
    if ((fp = fopen(fileName, "w+t")) == NULL) {
        printf("Cannot create the file: %s\n", fileName);
        exit(1);
    }
    fprintf(fp, "%d\n", nodeCount);
    for (int i = 0; i < nodeCount; i++) {
        for (int j = 0; j < nodeCount; j++) {
            fprintf(fp, "%d ", output[i * nodeCount + j]);
        }
        fprintf(fp, "\n");
    }
}