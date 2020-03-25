#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "problem.h"
#include <omp.h>

void SequentialSolution(FILE *file);

int main(int argc, char **arguments) {

    const char *filePath = arguments[1];
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        printf("Error: could not read file %s", filePath);
        exit(-1);
    }
    SequentialSolution(file);
}

void SequentialSolution(FILE *file) {

    struct ProblemInstance instance = readFromFile(file);

    bool *solution = malloc(sizeof(bool) * vertexCount);
    for (int n = 0; n < vertexCount; n++) solution[n] = false;
    clock_t begin = clock(); // Start of execution
    // Parallel Part of Algorithm with certain number of threads
#pragma omp parallel num_threads(2)
    {
        printf("Number of thread: %d \n", omp_get_thread_num());
#pragma omp single
        recursiveBruteForce(solution, 0, 0);
    }
    clock_t end = clock(); // End of execution

    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC; // Calculate time in seconds

    for (int i = 0; i < vertexCount; i++) {
        free(graphConnections[i]);
    }
    printf("Min Cut Solution: %f \n", minCutValue);

    printf("Runtime: %f s", time_spent);

    free(instance.graph.edges);
}


