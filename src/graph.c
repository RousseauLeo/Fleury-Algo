#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "graph.h"



Graph initGraph (int V, struct edge* e, int m, int * count)
{
	Graph g = malloc(V * sizeof (struct node));
	int i,j;
	int cID, csID;
	for(i = 0; i < V; ++i)
	{
		g[i].id = i;
		g[i].degree = count[i];
		g[i].around = malloc(g[i].degree * sizeof(int));
	}
	
	for(j = 0; j < m; ++j)
	{
		cID = e[j].u;
		csID = e[j].v;
		g[cID].around[g[cID].degree - count[cID]] = csID;
		g[csID].around[g[csID].degree - count[csID]] = cID;
		
		count[cID] --; count[csID] --;
	}
	
	return g;
}

void killGraph (Graph g)
{
}

int main()
{
	srand(time(NULL));
	struct edge ed[7];
	
	ed[ 0].u = 0; ed[ 0].v = 1;
	ed[ 1].u = 0; ed[ 1].v = 2;
	ed[ 2].u = 0; ed[ 2].v = 3;
	ed[ 3].u = 1; ed[ 3].v = 2;
	ed[ 4].u = 1; ed[ 4].v = 4;
	ed[ 5].u = 2; ed[ 5].v = 4;
	ed[ 6].u = 2; ed[ 6].v = 3;
	ed[ 7].u = 3; ed[ 7].v = 4;
	
	int count [5] = {3,3,4,3,3};
	int count2 [5] = {3,3,4,3,3};
	Graph g = initGraph(5,ed,7,count);
	
	int i,j;
	
	for(i = 0; i < 5; i++)
	{
		printf("%d : ",i);
		for(j = 0; j < count2[i]; j++) printf("%d ", g[i].around[j]);
		printf("\n");
	}
	
	printf("\n");
	
	return 0;
}
