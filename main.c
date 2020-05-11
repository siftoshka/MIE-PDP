#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "problem.h"
#include <omp.h>
#include "mpi.h"

void Computation(FILE *file);

int main(int argc, char **arguments) {

    const char *filePath = arguments[1];
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        printf("Error: could not read file %s", filePath);
        exit(-1);
    }

    if (argc == 1) {
        printf("Panic! expects filepath as second argument.\n");
        exit(-1);
    }

    readFromFile(file);

    bool *solution = (bool *) malloc(sizeof(bool) * vertexCount);
    for (int i = 0; i < vertexCount; i++)
        solution[i] = false;
    clock_t begin = clock(); // Start of execution

    MPI_Status status;

    // Initialize the MPI environment
    int provided;
    MPI_Init(&argc, &arguments);

    // Get the number of processes
    int mpiProcesses;
    MPI_Comm_size(MPI_COMM_WORLD, &mpiProcesses);

    // Get the rank of the process
    int mpiRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);

    int DONE = 0, WORKING = 1, FINISHED = 0;

    // This is master process
    if (mpiRank == 0) {
        // Initial work distribution
        for (int dest = 1; dest < mpiProcesses; dest++)
            MPI_Send(&minCutValue, vertexCount, MPI_C_BOOL, dest, WORKING, MPI_COMM_WORLD);


        // The number of slaves
        int workingSlaves = mpiProcesses - 1;

        while (workingSlaves > 0) { // the main loop
            recursiveBruteForceF(solution, 0, 0);
            float tmp;
            MPI_Recv(&tmp, 1, MPI_FLOAT, MPI_ANY_SOURCE, DONE, MPI_COMM_WORLD, &status);

            if (minCutValue > tmp) {// if there is more work to do
                minCutValue = tmp;
                MPI_Send(&minCutValue, vertexCount, MPI_C_BOOL, status.MPI_SOURCE, WORKING, MPI_COMM_WORLD);
            } else {
                MPI_Send(&minCutValue, vertexCount, MPI_C_BOOL, status.MPI_SOURCE, FINISHED, MPI_COMM_WORLD);
                workingSlaves--;
            }
        }
        clock_t end = clock(); // End of execution

        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC; // Calculate time in seconds

        printf("MinCut: %f\n", minCutValue);
        printf("Runtime: %f s", time_spent);

        // Slave processes
    } else {

        while (true) {

            MPI_Recv(solution, vertexCount, MPI_C_BOOL, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == FINISHED) break; // end of computation
            else if (status.MPI_TAG == WORKING) {
                // do the work
#pragma omp parallel for
                for (int depth = 0; depth < vertexCount; depth++)
                    recursiveBruteForceF(solution, 0, depth);

                MPI_Send(&minCutValue, 1, MPI_FLOAT, 0, DONE, MPI_COMM_WORLD);
            }
        }
    }

    // Finalize the MPI environment
    MPI_Finalize();

    free(solution);
    free(minCutArray);
}

/******************************************************************************************************************/

void Computation(FILE *file) {

    struct ProblemInstance instance = readFromFile(file);

    //bool *solution = (bool *) malloc(sizeof(bool) * vertexCount);
    //int depth = 0;
    //int cutSum = 0;
    //for (int n = 0; n < vertexCount; n++) solution[n] = false;

    clock_t begin = clock(); // Start of execution

    /**COMPUTER LAB 1**/
    //recursiveBruteForce(solution, cutSum, depth);

    /**COMPUTER LAB 2**/
    // Parallel Part of Algorithm with certain number of threads (Task-Parallelism)
//#pragma omp parallel
//    {
//        printf("Number of thread: %d \n", omp_get_thread_num());
//#pragma omp single
//        recursiveBruteForceOMPTask(solution, cutSum, depth);
//    }
    /**COMPUTER LAB 3**/
//    // Parallel Part of Algorithm with certain number of threads (Data-Parallelism)
//#pragma omp parallel for
//        // for cycle for data-parallelism
//        for (depth = 0; depth < vertexCount; depth++) {
//            recursiveBruteForceOMPData(globalSolution, globalCutSum, depth);
//        }

/******************************************************************************************************************/

    clock_t end = clock(); // End of execution

    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC; // Calculate time in seconds

//    for (int i = 0; i < vertexCount; i++) {
//        free(graphConnections[i]);
//    }
    //  printf("Min Cut Solution: %f \n", minCutValue);
    printf("Runtime: %f s", time_spent);

    // free(instance.graph.edges);
    // free(minCutArray);
    // free(solution);
}


