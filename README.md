# Floyd-Warshall-OMP

This code has been tested on `gcc-9.2` and `gcc-10`. To load `gcc-9.2`, use the command:
 
 ```module load gcc-9.2```
 
Use this command to compile:

```gcc-9.2 -fopenmp -Wall -std=c99 -o fw src/*.c```

Use this command to run the program:

```./fw```

This runs the code with the default values. Alternatively, use the following options to specify a value:

| Option         |    Usage      | Description          
|----------------|---------------|--------------------|   
| -r             | `./fw -r` |  Run benchmarking code
| -n             | `./fw -n1024` |  Number of vertices to be used to generate the graph
| -e             | `./fw -e0.5` |  Probability that an edge exists between vertices
| -b             | `./fw -b32` |  Size of the block
| -m             | `./fw -m1` |  Minimum number of threads
| -t             | `./fw -t5` |  Maximum number of threads
| -s             | `./fw -s` |  Save the outputs to a file
| -p             | `./fw -p` |  Print the outputs
| -v             | `./fw -v` |  Validate the outputs
| -i            | `./fw -i5` |  Number of iterations to run for each version

An example:

``` ./fw -n256 -e0.5 -b32 -m1 -t10 -v ```

None of these options is mandatory.