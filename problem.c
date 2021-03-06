#include "problem.h"
#include <omp.h>

int vertexCount = 0, subgroupSize = 0;
float minCutValue = 0, **graphConnections = NULL;
float globalCutSum = 0;
bool *globalSolution = NULL;
bool *minCutArray = NULL;

/******************************************************************************************************************/

// Declares instance variable
struct ProblemInstance instance = {
        .n =   0,
        .a =   0,
        .k =   0,
};

struct ProblemInstance readFromFile(FILE *file) {

    // Reads n from file
    instance.n = atoi(readWords(file));
    vertexCount = instance.n;
    instance.graph.nodeCount = instance.n;
    // Reads k from file
    instance.k = atoi(readWords(file));
    // Reads a from file
    instance.a = atoi(readWords(file));
    subgroupSize = instance.a;

    char *word;
    const size_t edge_size = sizeof(struct Edge);
    const unsigned int graph_grow_step = 16;
    unsigned int graph_capacity = graph_grow_step;
    unsigned int edge_count = 0;

    // Allocates graph
    instance.graph.edgeCount = 0;
    instance.graph.edges = (struct Edge *) malloc(graph_capacity * edge_size);

    minCutArray = (bool *) malloc(sizeof(bool) * vertexCount);
    globalSolution = (bool *) malloc(sizeof(bool) * vertexCount);
    minCutValue = 10000000;
    graphConnections = (float **) malloc(sizeof(double *) * vertexCount);
    for (int j = 0; j < vertexCount; j++) {
        graphConnections[j] = (float *) malloc(sizeof(double) * vertexCount);
        for (int i = 0; i < vertexCount; i++)
            graphConnections[j][i] = (float) 0;
    }

    // Reads edges:
    while (true) {
        // Reads a node "a" index
        word = readWords(file);

        if (word == NULL) break;
        // Parses node "a" from string to float
        instance.graph.edges[edge_count].nodeA = atoi(word);

        // Reads a node "b" index
        word = readWords(file);

        // Parses node "b" from string to float
        instance.graph.edges[edge_count].nodeB = atoi(word);

        // Reads a weight
        word = readWords(file);

        // Parses weight from string to float
        instance.graph.edges[edge_count].weight = atof(word);

        // Create graph connection
        graphConnections[instance.graph.edges[edge_count].nodeA][instance.graph.edges[edge_count].nodeB] = instance.graph.edges[edge_count].weight;

        // Continues counting
        edge_count++;

        // Expands graph's edge capacity, when limit has been reached
        if (edge_count == graph_capacity) {
            graph_capacity = graph_capacity + graph_grow_step;
            instance.graph.edges = (struct Edge *) realloc(instance.graph.edges, graph_capacity * edge_size);
        }
    }
    return instance;
}

/******************************************************************************************************************/

/**COMPUTER LAB 1**/
// Recursive Brute-Force
void recursiveBruteForce(bool *solution, float cutSum, int depth) {
    // In case if it is not correct
    if (checkPartialSolution(solution, depth))return;
    if (cutSum > minCutValue) return;

    if (depth == vertexCount) {
        minCutValue = cutSum;
        for (int i = 0; i < vertexCount; i++)
            minCutArray[i] = solution[i];
        return;
    }

    solution[depth] = false;
    recursiveBruteForce(solution, minCutSum(solution, depth + 1), depth + 1);

    solution[depth] = true;
    recursiveBruteForce(solution, minCutSum(solution, depth + 1), depth + 1);
}

/******************************************************************************************************************/

/**COMPUTER LAB 2**/
// Recursive Brute-Force with OpenMP Task Parallelism
void recursiveBruteForceOMPTask(bool *solution, float cutSum, int depth) {
    // In case if it is not correct
    if (checkPartialSolution(solution, depth))return;
    if (cutSum > minCutValue) return;

    if (depth == vertexCount) {
#pragma omp critical
        minCutValue = cutSum;
        for (int i = 0; i < vertexCount; i++)
#pragma omp critical
                minCutArray[i] = solution[i];
        return;
    }

    solution[depth] = false;
#pragma omp task
    recursiveBruteForce(solution, minCutSum(solution, depth + 1), depth + 1);

    solution[depth] = true;
#pragma omp task
    recursiveBruteForce(solution, minCutSum(solution, depth + 1), depth + 1);
}

/******************************************************************************************************************/

/**COMPUTER LAB 3**/
// Brute-Force with OpenMP Data Parallelism
void recursiveBruteForceOMPData(bool *solution, float cutSum, int depth) {
    // In case if it is not correct
    if (checkPartialSolution(solution, depth))return;
    if (cutSum > minCutValue) return;

    if (depth == vertexCount) {
        minCutValue = cutSum;
        for (int i = 0; i < vertexCount; i++)
            minCutArray[i] = solution[i];
        return;
    }
    globalCutSum = minCutSum(globalSolution, depth);
    recursiveBruteForce(globalSolution, globalCutSum, depth);
}

/******************************************************************************************************************/

// Check Particular Solution to how many 1 and 0 have
bool checkPartialSolution(const bool *solution, int depth) {
    int a = 0, n = 0;
    for (int i = 0; i < depth; i++) {
        if (solution[i]) a = a + 1;
        else n = n + 1;
        if (a > subgroupSize || n > (vertexCount - subgroupSize))
            return true;
    }
    return false;
}

/******************************************************************************************************************/

// Get a sum of Sub-Graph
float minCutSum(const bool *solution, int depth) {
    float sum = 0;
    for (int i = 0; i < depth; i++) {
        for (int j = 0; j < i; j++) {
            if (solution[j] != solution[i])
                sum += graphConnections[j][i];
        }
    }
    return sum;
}

/******************************************************************************************************************/

void recursiveBruteForceF(bool *solution, float cutSum, int depth) {
    // In case if it is not correct
    if (checkPartialSolution(solution, depth)) return;
    if (cutSum > minCutValue) return;

    if (depth == vertexCount) {
#pragma omp critical
        minCutValue = cutSum;
        for (int i = 0; i < vertexCount; i++)
            minCutArray[i] = solution[i];
    }

    solution[depth] = false;
    recursiveBruteForceF(solution, minCutSum(solution, depth + 1), depth + 1);

    solution[depth] = true;
    recursiveBruteForceF(solution, minCutSum(solution, depth + 1), depth + 1);
}