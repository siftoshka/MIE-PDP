#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "problem.h"
#include <omp.h>
#include </usr/local/opt/open-mpi/include/mpi.h>
void Computation(FILE *file);

int main(int argc, char **arguments) {

    const char *filePath = arguments[1];
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        printf("Error: could not read file %s", filePath);
        exit(-1);
    }
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    Computation(file);
}

void Computation(FILE *file) {

    struct ProblemInstance instance = readFromFile(file);

    bool *solution = malloc(sizeof(bool) * vertexCount);
    int depth = 0;
    int cutSum = 0;
    for (int n = 0; n < vertexCount; n++) solution[n] = false;
    clock_t begin = clock(); // Start of execution

    /**COMPUTER LAB 1**/
    //recursiveBruteForce(solution, cutSum, depth);

    /**COMPUTER LAB 2**/
    // Parallel Part of Algorithm with certain number of threads (Task-Parallelism)
//#pragma omp parallel num_threads(2)
//    {
//        printf("Number of thread: %d \n", omp_get_thread_num());
//#pragma omp single
//        recursiveBruteForceOMPTask(solution, cutSum, depth);
//    }
    /**COMPUTER LAB 3**/
    // Parallel Part of Algorithm with certain number of threads (Data-Parallelism)
//#pragma omp parallel for num_threads(2)
//    {
//        // for cycle for data-parallelism
//        for (int i = 0; i < vertexCount; i++) {
//            recursiveBruteForceOMPData(globalSolution, globalCutSum, depth);
//        }
//    }
    /**COMPUTER LAB 4**/
    // Parallel Part of Algorithm using Open MP with MPI.
//#pragma omp parallel for num_threads(2)
//    {
//        // for cycle for data-parallelism
//        for (int i = 0; i < vertexCount; i++) {
//            recursiveBruteForceOMPData(globalSolution, globalCutSum, depth);
//        }
//    }

    clock_t end = clock(); // End of execution

    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC; // Calculate time in seconds

    for (int i = 0; i < vertexCount; i++) {
        free(graphConnections[i]);
    }
    printf("Min Cut Solution: %f \n", minCutValue);

    printf("Runtime: %f s", time_spent);

    free(instance.graph.edges);
    free(minCutArray);
    free(solution);
}


