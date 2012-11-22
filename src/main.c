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
    int n = 5, m = 10;
    struct edge_t e[10];
    int count[5];

    /* Solution finale */
    int out[10];
    int size = 0;

    /* variables temporaires */
    int i = 0;
	/*
    g = (struct node_t *)malloc(n * sizeof(struct node_t));

    e[0].u = 0; e[0].v = 1;
    e[1].u = 1; e[1].v = 2;
    e[2].u = 2; e[2].v = 3;
    e[3].u = 3; e[3].v = 0;

    e[4].u = 1; e[4].v = 4;
    e[5].u = 2; e[5].v = 4;

    e[6].u = 0; e[6].v = 4;
    e[7].u = 0; e[7].v = 2;

    e[8].u = 3; e[8].v = 1;
    e[9].u = 3; e[9].v = 4;

    count[0] = 4;
    count[1] = 4;
    count[2] = 4;
    count[3] = 4;
    count[4] = 4;
	*/
    printf("Initialisiation ...\n");
	n = graph_read_from_std(g);
    /*graph_create_from_list(g, n, m, e, count);*/

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
        printf("Il n'existe pas de solutions.\n");

    printf("Destruction ...\n");

    graph_destroy(g, 3);

    free(g);

    printf("Done.\n");

    return 0;
}
