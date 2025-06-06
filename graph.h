// #include <stdio.h>
// #include <stdlib.h>
#include <stdbool.h>

typedef struct edge
{
    double speed;
    int len;
} EDGE;

typedef struct graph
{
    int n_verticles;
    EDGE **graph_matrix;
    int *verticles;
    bool *visited;
} GRAPH;
