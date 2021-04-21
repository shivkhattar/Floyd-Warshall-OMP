extern void print(const int *output, const int nodeCount);

extern void save(const int *output, const int nodeCount, const char *fileName);

double getAverageTime(double times[], int size);

void getOptions(int argc, char **argv, int *nodeCount, double *probability, int *blockSize, int *minNumThreads,
                int *maxNumThreads, int *maxIterations, bool *save, bool *print, bool *validate, bool *benchmark);