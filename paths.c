#include "paths.h"

QUEUE *queue_create()
{
    QUEUE *queue = malloc(sizeof(QUEUE));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void queue_add(QUEUE *queue, int num, EDGE *edge)
{
    if (!queue)
    {
        exit(EXIT_FAILURE);
    }
    NODE *insert = def_node_construct(num);
    insert->edge = edge;
    if (queue->size == 0)
    {
        queue->head = insert;
        queue->tail = insert;
    }
    else
    {
        queue->tail->next = insert;
        queue->tail = insert;
    }
    queue->size++;
}

NODE *queue_take(QUEUE *queue)
{
    if (queue->size > 0)
    {
        NODE *curr = queue->head;
        // if (queue->head->next != NULL)
        queue->head = queue->head->next;
        queue->size--;
        return curr;
    }
    return NULL;
}

NODE *def_node_construct(int src)
{
    NODE *node_list = malloc(sizeof(NODE));
    node_list->edge = calloc(1, sizeof(EDGE));
    node_list->num = src;
    node_list->next = NULL;
    return node_list;
}

void destroy_node(NODE *node)
{
    free(node->edge);
    free(node);
}

LIST *def_list_construct(int src)
{
    LIST *list = malloc(sizeof(LIST));
    list->head = def_node_construct(src);
    list->visited = calloc(10, sizeof(bool)); // 10 переделать
    list->visited[src] = true;
    list->tail = NULL;
    list->next = NULL;
    list->path = 0;
    list->time = 0;
    return list;
}

PATHS *def_path_construct()
{
    PATHS *paths = malloc(sizeof(PATHS));
    paths->count = 0;
    paths->first = NULL;
    paths->last = NULL;
    return paths;
}

void insert_in_list(LIST *list, int num, EDGE *edge)
{
    if (!list)
    {
        exit(EXIT_FAILURE);
    }
    NODE *insert = def_node_construct(num);
    insert->edge = edge;
    if (list->head->next == NULL)
    {
        list->head->next = insert;
        list->tail = insert;
    }
    else
    {
        list->tail->next = insert;
        list->tail = insert;
    }
    list->path += edge->len;
    list->time += edge->len / edge->speed;
    list->visited[num] = true;
}

void insert_in_path(PATHS *path, LIST *insert)
{
    if (!path || !insert)
    {
        exit(EXIT_FAILURE);
    }
    if (path->count == 0)
    {
        path->first = insert;
        path->last = insert;
    }
    else
    {
        path->last->next = insert;
        path->last = insert;
    }
    path->count++;
}

LIST *copy_list(LIST *src, int num) // num - до какой вершины копировать список
{
    if (!src)
        return NULL;
    LIST *res = def_list_construct(src->head->num);
    if (src->head->num == num)
        return res;
    NODE *current = src->head->next;
    while (current != NULL)
    {
        // NODE *insert = def_node_construct();
        insert_in_list(res, current->num, current->edge);
        if (current->num == num)
            break;
        current = current->next;
    }
    return res;
}

bool is_visited(LIST *src, int num)
{
    if (!src)
        return false;
    for (NODE *curr = src->head; curr != NULL; curr = curr->next)
    {
        if (curr->num == num)
            return true;
    }
    return false;
}

void func(LIST *path, bool *visited)
{
    if (path && path->head && path->head->next)
    {
        for (NODE *curr = path->head->next->next; curr != NULL; curr = curr->next)
        {
            visited[curr->num] = false;
        }
    }
}

void Dfs(int src, int res, PATHS *path, size_t n, EDGE **graph, int *verticles, bool *visited)
{
    if (src == res)
        return;

    if (!path)
    {
        puts("noo(");
        exit(EXIT_FAILURE);
    }
    visited[src] = true;
    for (int i = 0; i < n; i++)
    {

        if (graph[src][i].len > 0)
        {
            if (!visited[i])
            {
                LIST *new_list = NULL;

                if (verticles[src] > 0)
                {
                    new_list = copy_list(path->last, src);
                    insert_in_list(new_list, i, &(graph[src][i]));
                    insert_in_path(path, new_list);
                }
                else
                {
                    if (!is_visited(path->last, i))
                    {
                        if (path->count == 0)
                        {
                            new_list = def_list_construct(src);
                            insert_in_list(new_list, i, &(graph[src][i]));
                            insert_in_path(path, new_list);
                        }
                        else
                            insert_in_list(path->last, i, &(graph[src][i]));
                    }
                }

                verticles[src]++;
                Dfs(i, res, path, n, graph, verticles, visited);
                visited[i] = false;
            }
        }
    }
}

void Bfs(int src, int res, PATHS *path, size_t n, EDGE **graph)
{
    bool *visited = calloc(n, sizeof(bool));
    NODE *vert;
    QUEUE *queue = queue_create();
    visited[src] = true;
    EDGE src_edge = {0.0, 0};
    queue_add(queue, src, &src_edge);
    int *verticles = calloc(n, sizeof(int));
    while (queue->size > 0)
    {
        vert = queue_take(queue);
        if (vert->num == res)
            continue;
        for (int i = 0; i < n; i++)
        {
            if (graph[vert->num][i].len > 0)
            {
                queue_add(queue, i, &(graph[vert->num][i]));
                LIST *new_list = NULL;

                if (verticles[vert->num] > 0)
                {
                    for (LIST *curr = path->first; curr != NULL; curr = curr->next)
                        if (curr->tail->num == vert->num && !(curr->visited[i]))
                        {
                            new_list = copy_list(curr, vert->num);
                            insert_in_list(new_list, i, &(graph[vert->num][i]));
                            insert_in_path(path, new_list);
                        }
                }
                else
                {
                    if (path->count == 0)
                    {
                        new_list = def_list_construct(src);
                        insert_in_list(new_list, i, &(graph[src][i]));
                        insert_in_path(path, new_list);
                    }
                    else // добавить поиск списка по крайней вершине (проверять на совпадение с vert->num)
                    {
                        for (LIST *curr = path->first; curr != NULL; curr = curr->next)
                            if (curr->tail->num == vert->num && !(curr->visited[i]))
                                insert_in_list(curr, i, &(graph[vert->num][i]));
                    }
                }

                visited[i] = true;
                verticles[vert->num]++;
            }
        }
    }
}

/*Демонстрация графа как матрицы смежности*/
void show_graph(int v, EDGE **mass)
{
    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < v; j++)
        {
            printf("%d\t", mass[i][j].len);
        }
        putchar('\n');
    }
}

void show_paths(PATHS *paths)
{
    int count = 0;
    for (LIST *curr = paths->first; curr != NULL; curr = curr->next)
    {
        count++;
        printf("Путь %d: ", count);

        for (NODE *temp = curr->head; temp != NULL; temp = temp->next)
            printf("%d->", temp->num);
        printf(": %d\n", curr->path);
    }
}

int main()
{
    PATHS *path = def_path_construct();
    int *verticles = calloc(7, sizeof(int));
    bool *visited = calloc(7, sizeof(bool));
    EDGE **graph = malloc(7 * sizeof(EDGE *));
    if (!graph)
    {
        puts("noo(");
        exit(EXIT_FAILURE);
    }
    for (int v = 0; v < 7; v++)
    {
        graph[v] = malloc(7 * sizeof(EDGE));
        if (!graph[v])
            exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            graph[i][j].len = 0;
            graph[i][j].speed = 0.0;
        }
    }

    graph[0][1].len = 15;
    graph[0][2].len = 10;
    graph[0][3].len = 25;
    graph[1][4].len = 26;
    graph[2][4].len = 8;
    graph[3][5].len = 17;
    graph[4][5].len = 14;
    graph[4][6].len = 19;
    graph[5][6].len = 33;

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
            if (graph[i][j].len > 0)
                graph[j][i].len = graph[i][j].len;
    }

    show_graph(7, graph);
    Dfs(0, 5, path, 7, graph, verticles, visited);
    // Bfs(0, 5, path, 7, graph);

    printf("count: %d\n", path->count);
    show_paths(path);
    free(verticles);
    free(visited);

    return 0;
}
