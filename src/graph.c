#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/*
 * g graph to allocate
 * n the number of nodes
 * m the number of edges
 * e all the edges in the graph
 * count number of edges for each node 
 */ 
int graph_create_from_list (struct node_t * g, int n, int m, struct edge_t * e, int * count)
{
    int i = 0, j = 0;
    struct node_t * cID = 0,  * csID = 0;
	
    if(n == 0 || g == 0)
    {
		printf("erreur: le graphe est vide !\n");
        return -1;
	}
	
    for(i = 0; i < n; ++i)
    {
        g[i].id = i;
        g[i].degree = 0;
        g[i].neighbours = (struct node_t **)malloc(count[i] * sizeof(struct node_t *));
    }
	
    /*
     * Creation d'un graphe orienté à partir des aretes.
     * Les aretes ne sont utilisées que dans un sens.
     */
    for(j = 0; j < m; ++j)
    {
        cID = &g[e[j].u];
        csID = &g[e[j].v];

        if(cID == csID)
        {
			printf("erreur: un sommet ne peut pointer sur lui-même !\n");
			return -1;
		}
        
        g[cID->id].neighbours[g[cID->id].degree++] = csID;
    }
    
    return 0;
}


int graph_read_from_std (struct node_t ** gp, int * np, int * mp)
{
	int n = 0, m = 0, s = 0, c = 0, i = 0;
	int count;
	int * buf;
	int * ned;
	
	int ret = 0;
	
	struct edge_t * e = 0;
	
	scanf("%d", &n);
	
	buf = (int *)malloc(n * sizeof(int));
	ned = (int *)malloc(n * sizeof(int));
	e   = (struct edge_t *)malloc(n * n * sizeof(struct edge_t));
	*gp	= (struct node_t *)malloc(n * sizeof(struct node_t));
	
	for(c = 0; c < n; c++)
	{
		count = 0;
		
		scanf("%d" ,&s);
		
		do {
			m++;
			/* On retire 1, c'est un peu bizarre mais le sujet dit que
			 * c'est numéroté de 1 à n, alors bon ... */
			buf[count++] = s - 1;
			scanf("%d", &s);
		} while(s != -1 );
		
		ned[c] = count + 1;
		
		for(i = 0; i < count; i++)
		{
			e[m - count + i].u = c;
			e[m - count + i].v = buf[i];
		}
	}
	
	*np = n;
	*mp = m / 2;
	
	ret = graph_create_from_list(*gp, n, m, e, ned);
	
	if(m % 2 != 0)
	{
		printf("erreur: le nombre d'arêtes doit être pair !\n");
		ret = -1;
	}
	
	free(buf);
    free(ned);
	free(e);
	
	return ret;
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

void graph_darken_edge(struct node_t * a, struct node_t * b, int x)
{
    int i = 0;
    
    for(i = 0; i < a->degree; ++i)
    {
        if(a->neighbours[i]->id == b->id)
        {
            a->neighbours[i] = a->neighbours[--a->degree];
            break;
        }
    }

    for(i = 0; i < b->degree; ++i)
    {
        if(b->neighbours[i]->id == a->id)
        {
            b->neighbours[i] = b->neighbours[--b->degree];
            break;
        }
    }
}

void graph_undarken_edge(struct node_t * a, struct node_t * b)
{
    a->neighbours[a->degree++] = b;
    b->neighbours[b->degree++] = a;
}

/* Appel récursif pour le parfours en profondeur */
int __dfsRec (struct node_t * n, int * status, int (*process)(struct node_t * n, void * data), void * data)
{
    int i = 0, res = 0;

    status[n->id] = 1;

    if(process)
        res = process(n, data);
    
	for(i = 0; i < n->degree && res == 0; ++i)
	{
		/* Si le noeud est accessible */
		if(status[n->neighbours[i]->id] == 0)
			res = __dfsRec(n->neighbours[i], status, process, data);
	}
	
	return res;
}

int graph_dfs_func (struct node_t * gorig, int n, int (*process)(struct node_t * n, void * data), void * data)
{
    int i = 0, res = 0;
    int * status = (int *)malloc(n * sizeof(int));

    for(i = 0; i < n; ++i)
        status[i] = 0;
        
	res = __dfsRec(gorig, status, process, data);

    free(status);

    return res;
}

int __display_func (struct node_t * n, void * data)
{
    int i = 0;

    for(i = 0; i < n->degree; ++i)
        printf("%d -> %d\n", n->id, n->neighbours[i]->id);
    
    return 0;
}

int graph_dfs_display (struct node_t * g, int n)
{
	printf("N = %d\n", n);
    return graph_dfs_func(&g[0], n, __display_func, 0);
}
