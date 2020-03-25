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
extern bool *minCutArray;

struct Edge {
    unsigned int node_a;
    unsigned int node_b;
    float weight;
};

struct Graph {
    unsigned int node_count;
    unsigned int edge_count;
    struct Edge *edges;
};

struct ProblemInstance {
    int n; // Count of nodes in the original graph
    int a; // Count of nodes in a resulted graph after cut
    int k; // Maximum degree of any node
    struct Graph graph;
};

struct ProblemInstance readFromFile(FILE *file);

double minCutSum(const bool *solution, int depth);

void recursiveBruteForce(bool *solution, float cutSum, int depth);

bool checkPartialSolution(const bool *solution, int depth);

// Ends header guard
#endif