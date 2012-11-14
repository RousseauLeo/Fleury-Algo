#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

void display(struct node_t * g, int x)
{
	int i = 0;
	
	for(i = 0; i < g[x].degree; ++i)
	{
		printf("%d -> %d\n", x, g[x].neighbours[i]);
	}
}

int main (int argc, char **argv)
{
	struct node_t * g = 0;
	
	/* Definition d'un graphe triangulaire orienté */
	const int n = 3, m = 3;
	struct edge_t e[3];
	int count[3];

	int i = 0;
	
	e[0].u = 0;	e[0].v = 1;
	e[1].u = 1; e[1].v = 2;
	e[2].u = 2;	e[2].v = 0;
	
	count[0] = 2;
	count[1] = 2;
	count[2] = 2;
	
	printf("Initialisiation ...\n");
	
	graph_create_from_list(&g, n, m, e, count);
	
	printf("Affichage ...\n");
	
	graph_dfs(g, n, display);
	
	printf("Destruction ...\n");
	
	graph_destroy(&g, 3);
	
	printf("Done.\n");
	
	return 0;
}
