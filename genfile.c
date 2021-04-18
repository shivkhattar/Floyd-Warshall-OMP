#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_DISTANCE 20
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: ./genfile name nodes\n");
        printf("name: the name of the file to be generated\n");
        printf("nodes: the number of nodes to be generated\n");
        exit(1);
    }
    FILE *fp;
    if ((fp = fopen(argv[1], "w+t")) == NULL) {
        printf("Cannot create the file %s\n", argv[1]);
        exit(1);
    }

    int nodeCount = atoi(argv[2]);

    fprintf(fp, "%d\n", nodeCount);
    for(int i = 0; i < nodeCount; i++) {
        for(int j = 0; j < nodeCount; j++) {
            if(i == j) {
                fprintf(fp, "%d ", 0);
            } else {
                fprintf(fp, "%d ", 1 + (rand() % MAX_DISTANCE));
            }
        }
        fprintf(fp, "\n");
    }

    printf("Generated %d graph nodes and stored in file %s\n", nodeCount, argv[1]);

    fclose(fp);
    return 0;
}
