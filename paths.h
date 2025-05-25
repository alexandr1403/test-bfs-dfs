#ifndef CONT_H_
#define CONT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graph.h"

/*Узел*/
typedef struct node
{
    EDGE *edge;
    int num;
    struct node *next;
} NODE;

/*Список*/
typedef struct list
{
    NODE *head, *tail;
    int path, time;
    bool *visited;
    struct list *next;
} LIST;

typedef struct paths
{
    LIST *first, *last;
    int count;
} PATHS;

/*Очередь вершин*/
typedef struct queue
{
    int size;
    NODE *head, *tail;
} QUEUE;

/*Прототипы функций*/

NODE *def_node_construct();
LIST *def_list_construct();
PATHS *def_path_construct();
void insert_in_list(LIST *list, int num, EDGE *edge);
void insert_in_path(PATHS *path, LIST *insert);
LIST *copy_list(LIST *src, int num);

#endif
