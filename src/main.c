#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

int pick_next(int current, struct node_t * g, int n, int * cc)
{
    int i = 0;
    int next = -1;
    int count = 0;
    int found = 0;

    int tmpCc = 0;

    /* Pour chaque voisins du noeud en cours ... */
    for(i = 0; i < g[current].degree && !found; ++i)
    {
        /*
         * Si le noeud est accessible (l'arête est claire):
         * On vérifie que cette arête n'est pas un isthme, c'est à dire
         * que si on la supprime, on conserve le nombre de composantes
         * connexes dans le graphe.
         *
         * Toutefois, si en supprimant l'arête on ramène le degré du noeud
         * courent à 0, alors le nombre de composantes connexes dans le graphe
         * doit augmenter strictement de 1. (tmpCC vaut alors 1)
         */
        if(g[current].neighbours[i] >= 0)
        {
            next = g[current].neighbours[i];

            graph_darken_edge(&g[current], &g[next]);

            if(g[current].degree == 0)
                tmpCc = 1;

            count = graph_dfs_count(g, n);

            if(count == *cc + tmpCc)
            {
                *cc += tmpCc;
                found = 1;
            }
            else
            {
                /* Si l'arête est un isthme alors on ne la supprime pas et on
                 * passe au noeud suivant */
                graph_undarken_edge(&g[current], &g[next]);
                next = -1;
            }
        }
    }

    return next;
}

int fleury(struct node_t * g, int n, int * out)
{
    int traversed = 0;
    int next = 0;
    int cc = 1;

    out[traversed] = 0;

    /* Tant qu'on peut visiter un noeud sans passer
     * par une arête noircie, on continue. */
    while(next >= 0)
    {
        /* Trouver le prochain noeud à visiter et noircir l'arête */
        next = pick_next(next, g, n, &cc);

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
	
    printf("Initialisiation ...\n");
    
	ret = graph_read_from_std(&g, &n, &m);
	
	if(ret == 0)
	{
		printf("G(%d, %d) créé\n", n , m);
		
		out = (int *)malloc((m + 1) * sizeof(int));

		printf("Affichage ...\n");

		graph_dfs_display(g, n);

		printf("Nombre de composantes connexes: %d\n", graph_dfs_count(g, n));

		printf("Application de l'algorithme de fleury sur G(%d, %d) ...\n", n, m);
		size = fleury(g, n, out);

		if(size == m)
		{
			printf("Solution trouvée !\n");
			for(i = 0; i < size; ++i)
				printf("%d ", out[i]);
			printf("\n");
		}
		else
			printf("Il n'existe pas de solutions. (les sommets ne sont pas de dégrés pair)\n");
	}
	else
		printf("Une erreur est survenue ...\n");
		
    printf("Destruction ...\n");

    graph_destroy(g, n);

    free(g);
    
    free(out);

    printf("Done.\n");

    return 0;
}
