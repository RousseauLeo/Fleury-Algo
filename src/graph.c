#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

void graph_create_from_list (struct node_t ** gp, int n, int m, struct edge_t * e, int * count)
{	
	int i = 0, j = 0;
	int cID = 0, csID = 0;
	
	struct node_t * g = 0;
	
	if(n == 0)
	{
		*gp = 0;
		return;
	}
	
	*gp = malloc(n * sizeof (struct node_t));
	
	g = *gp;
	
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

void graph_destroy (struct node_t ** gp, int n)
{
	int i = 0;
	struct node_t * g = 0;
	
	g = *gp;
	
	if(gp == 0)
		return;
	
	for(i = 0; i < n; ++i)
	{
		if(g[i].neighbours != 0)
			free(g[i].neighbours);
	}
	
	free(g);
	
	*gp = 0;
}

void dfsRec(struct node_t *g, int x, int *status, void process(struct node_t *g, int x))
{
	int i = 0;
	
	status[x] = 1;
	
	process(g, x);
	
	for(i = 0; i < g[x].degree; ++i)
	{
		if(status[g[x].neighbours[i]] == 0)
			dfsRec(g, g[x].neighbours[i], status, process); 
	}
}

void graph_dfs(struct node_t * g, int n, void process(struct node_t *g, int x))
{
	int i = 0;
	int * status = (int *)malloc(n * sizeof(int));
	
	for(i = 0; i < n; ++i)
		status[i] = 0;
	
	status[0] = 1;
	
	dfsRec(g, 0, status, process);
	
	free(status);
}
