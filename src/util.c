#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

void getOptions(int argc, char **argv, int *nodeCount, double *probability, int *blockSize, int *minNumThreads,
                int *maxNumThreads, int *maxIterations, bool *save, bool *print, bool *validate, bool *benchmark) {
    int opt = -1;
    while ((opt = getopt(argc, argv, "n:e:b:t:m:spvri:")) != -1) {
        switch (opt) {
            case 'r':
                *benchmark = true;
                break;
            case 'n':
                sscanf(optarg, "%d", nodeCount);
                break;
            case 'e':
                sscanf(optarg, "%lf", probability);
                break;
            case 'b':
                sscanf(optarg, "%d", blockSize);
                break;
            case 'm':
                sscanf(optarg, "%d", minNumThreads);
                break;
            case 't':
                sscanf(optarg, "%d", maxNumThreads);
                break;
            case 's' :
                *save = true;
                break;
            case 'p':
                *print = true;
                break;
            case 'v':
                *validate = true;
                break;
            case 'i':
                sscanf(optarg, "%d", maxIterations);
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

int comparator(const void *a, const void *b) {
    double *x = (double *) a;
    double *y = (double *) b;
    if (*x < *y) return -1;
    else if (*x > *y) return 1;
    return 0;
}

double getAverageTime(double times[], int size) {
    double avgTime = 0;
    if (size > 3) {
        // Remove outliers when size > 3
        qsort(times, size, sizeof(double), comparator);
        for (int i = 1; i < size - 1; i++) {
            avgTime += times[i];
        }
        avgTime /= (size - 2);
    } else {
        for (int i = 0; i < size; i++) {
            avgTime += times[i];
        }
        avgTime /= size;
    }
    return avgTime;
}

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