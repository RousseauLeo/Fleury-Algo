#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

int is_not_isthme_bfs (struct node_t * gorig, int n, int src)
{
    int i = 0;
    int * status = (int *)malloc(n * sizeof(int));
    
	int qsize = 0;
    struct node_t ** queue  = (struct node_t **)malloc(n * sizeof(struct node_t *));
	struct node_t * t = 0;
	
    for(i = 0; i < n; ++i)
        status[i] = 0;
    
    queue[qsize++] = gorig;
    status[gorig->id] = 1;
    
	while(qsize > 0)
	{
		t = queue[--qsize];
		
		if(t->id == src)
			return 1;
		
		for(i = 0; i < t->degree; ++i)
		{
			if(status[t->neighbours[i]->id] == 0)
			{
				status[t->neighbours[i]->id] = 1;
				queue[qsize++] = t->neighbours[i];
			}
		}
	}

	free(queue);
    free(status);

    return 0;
}

int pick_next(const int current, struct node_t * g, int n)
{
    int i = 0;
    int next = -1;
    
	if(g[current].degree == 1)
	{
		next = g[current].neighbours[0]->id;
		
		/* Visiter l'arête */
		graph_darken_edge(&g[current], g[current].neighbours[0], 0);
	}
	else
	{
		/* Pour chaque voisins du noeud en cours ... */
		for(i = 0; i < g[current].degree; ++i)
		{
			next = g[current].neighbours[i]->id;
			
			graph_darken_edge(&g[current], g[current].neighbours[i], i);
			
			if(is_not_isthme_bfs(&g[next], n, g[current].id))
				break;
		
			graph_undarken_edge(&g[current], &g[next]);
			next = -1;
		}
	}

    return next;
}

int fleury(struct node_t * g, int n, int * out)
{
    int traversed = 0;
    int next = 0;

    out[traversed] = 0;

    /* Tant qu'on peut visiter un noeud sans passer
     * par une arête noircie, on continue. */
    while(next >= 0)
    {
        /* Trouver le prochain noeud à visiter et noircir l'arête */
        next = pick_next(next, g, n);

        /* Si on a trouver un noeud, alors on l'ajoute à la liste des noeuds
         * visités. */
        if(next >= 0)
            out[++traversed] = next;
    }

    return traversed;
}

int main (int argc, char **argv)
{
    struct node_t * g = 0;
	
    /* Definition d'un graphe de test*/
    int n = 0, m = 0;

    /* Solution finale */
    int * out = 0;
    int size = 0;

    /* variables temporaires */
    int i = 0;
    int ret = 0;
	
    /* printf("Initialisiation ...\n"); */
    
	ret = graph_read_from_std(&g, &n, &m);
	
	if(ret == 0)
	{
		if(n >= 3)
		{
			/* printf("G(%d, %d) créé\n", n , m); */
			
			out = (int *)malloc((m + 1) * sizeof(int));

			/* printf("Affichage ...\n"); */

			/* graph_dfs_display(g, n); */

			printf("Application de l'algorithme de fleury sur G(%d, %d) ...\n", n, m);
			size = fleury(g, n, out);
			
			if(size == m)
			{
				/* printf("Solution trouvée !\n"); */
				
				/* On ajoute 1, c'est un peu bizarre mais le sujet dit que
				 * c'est numéroté de 1 à n, alors bon ... */
				for(i = 0; i <= size; ++i)
					printf("%d ", out[i] + 1);
				printf("\n");
			}
			else
				printf("Il n'existe pas de solutions : tous les sommets ne sont pas de dégrés pair.\n");
		}
		else
			printf("Il n'existe pas de solutions : moins de 3 sommets\n");
	}
	else
		printf("Une erreur est survenue ...\n");
		
    /* printf("Destruction ...\n"); */

    graph_destroy(g, n);

    free(g);
    
    free(out);

    /* printf("Done.\n"); */

    return 0;
}
