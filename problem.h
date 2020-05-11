// Begins header guard
#ifndef PDP_PROBLEM
#define PDP_PROBLEM

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "fileparse.h"

extern int vertexCount, subgroupSize;
extern float minCutValue, **graphConnections;
extern float globalCutSum;
extern bool *globalSolution;
extern bool *minCutArray;

struct Edge {
    unsigned int nodeA;
    unsigned int nodeB;
    float weight;
};

struct Graph {
    unsigned int nodeCount;
    unsigned int edgeCount;
    int* nodes;
    struct Edge *edges;
};

struct ProblemInstance {
    int n; // Count of nodes in the original graph
    int a; // Count of nodes in a resulted graph after cut
    int k; // Maximum degree of any node
    struct Graph graph;
};

struct ProblemSolution {
    int *array;
    int size;
    float cost;
    bool isValid;
};

struct ProblemInstance readFromFile(FILE *file);

float minCutSum(const bool *solution, int depth);

void recursiveBruteForce(bool *solution, float cutSum, int depth);

void recursiveBruteForceF(bool *solution, float cutSum, int depth);

void recursiveBruteForceOMPTask(bool *solution, float cutSum, int depth);

void recursiveBruteForceOMPData(bool *solution, float cutSum, int depth);

bool checkPartialSolution(const bool *solution, int depth);

// Ends header guard
#endif