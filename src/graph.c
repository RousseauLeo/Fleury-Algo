#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

void graph_create_from_list (struct node_t * g, int n, int m, struct edge_t * e, int * count)
{
    int i = 0, j = 0;
    int cID = 0, csID = 0;

    if(n == 0 || g == 0)
        return;

    for(i = 0; i < n; ++i)
    {
        g[i].id = i;
        g[i].degree = count[i];
        g[i].neighbours = malloc(g[i].degree * sizeof(int));
    }

    /*
     * Creation d'un graphe orienté à partir des aretes.
     * Les aretes ne sont utilisées que dans un sens.
     */
    for(j = 0; j < m; ++j)
    {
        cID = e[j].u;
        csID = e[j].v;

        g[cID].neighbours[g[cID].degree - count[cID]] = csID;
        g[csID].neighbours[g[csID].degree - count[csID]] = cID;

        count[cID]--; count[csID]--;
    }
}

void graph_destroy (struct node_t * g, int n)
{
    int i = 0;

    if(g == 0)
        return;

    for(i = 0; i < n; ++i)
    {
        if(g[i].neighbours != 0)
            free(g[i].neighbours);
    }
}


void graph_darken_edge(struct node_t * a, struct node_t * b)
{
    int i = 0;

    for(i = 0; i < a->degree; ++i)
    {
        if(a->neighbours[i] == b->id)
        {
            a->neighbours[i] = -1;
            a->neighbours[i] = a->neighbours[a->degree-1];
            a->degree--;
        }
    }

    for(i = 0; i < b->degree; ++i)
    {
        if(b->neighbours[i] == a->id)
        {
            b->neighbours[i] = -1;
            b->neighbours[i] = b->neighbours[b->degree-1];
            b->degree--;
        }
    }
}

void graph_undarken_edge(struct node_t * a, struct node_t * b)
{
    a->neighbours[a->degree] = b->id;
    b->neighbours[b->degree] = a->id;
    a->degree++;
    b->degree++;
}

/* Appel récursif pour le parfours en profondeur */
void __dfsRec (struct node_t *g, int x, int *status, void process(struct node_t *g, int x))
{
    int i = 0;

    status[x] = 1;

    if(process)
        process(g, x);

    for(i = 0; i < g[x].degree; ++i)
    {
        /* Si le noeud est accessible */
        if(g[x].neighbours[i] > 0)
        {
            if(status[g[x].neighbours[i]] == 0)
                __dfsRec(g, g[x].neighbours[i], status, process);
        }
    }
}

int graph_dfs_func (struct node_t * g, int n, void process(struct node_t *g, int x))
{
    int i = 0, count = 0;
    int * status = (int *)malloc(n * sizeof(int));

    for(i = 0; i < n; ++i)
        status[i] = 0;

    for(i = 0; i < n; ++i)
    {
        if(status[i] == 0)
        {
            count ++;
            __dfsRec(g, 0, status, process);
        }
        status[0] = 2;
    }

    free(status);

    return count;
}

int graph_dfs_count (struct node_t * g, int n)
{
    return graph_dfs_func(g, n, 0);
}

void __display_func (struct node_t * g, int x)
{
    int i = 0;

    for(i = 0; i < g[x].degree; ++i)
    {
        printf("%d -> %d\n", x, g[x].neighbours[i]);
    }
}

int graph_dfs_display (struct node_t * g, int n)
{
    return graph_dfs_func(g, n,  __display_func);
}
