/*
   -----------------------------------------------------------------------------
   Copyright (c) 2012 Léo Rousseau and Aurélien Cavelan
   
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   
   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
   -----------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "graph.h"

/* Parcours dfs à l'aide d'une pile afin de trouver un chemin entre
 * gorig et g[src] */
int is_not_isthme_dfs (struct node_t * gorig, int n, int src)
{
    int i = 0;
    
    /* Tableau d'états pour marquer le passage dans chaque sommet */
    int * status = (int *)malloc(n * sizeof(int));
    
    /* Création d'une pile */
	int tsize = 0;
    struct node_t ** stack  = (struct node_t **)malloc(n * sizeof(struct node_t *));
	struct node_t * t = 0;
	
	/* Aucun sommet n'est visité (0: non visité, 1: visité) */
    memset(status, 0, n * sizeof(int));
    
    /* Le sommet de départ est déjà visité */
    stack[tsize++] = gorig;
    status[gorig->id] = 1;
    
    /* Tant qu'il y a des sommets dans la pile */
	while(tsize > 0)
	{
		/* On récupère le sommet du dessus (pop) */
		t = stack[--tsize];
		
		/* Si c'est le noeud recherché on retourne vrai */
		if(t->id == src)
		{
			free(stack);
			free(status);
			return 1;
		}
		
		/* Pour chaque voisin de t ...
		 * Ici une astuce est de parcourir les voisins (triés) à
		 * l'envers. Cela permet permet sur certains graphes de réduire
		 * fortement le nombre d'itérations (ex: les Kn) */
		for(i = t->degree-1; i >= 0; --i)
		{
			if(status[t->neighbours[i]->id] == 0)
			{
				status[t->neighbours[i]->id] = 1;
				
				/* On ajoute le sommet en haut de la pile (push) */
				stack[tsize++] = t->neighbours[i];
			}
		}
	}
	
	/* On libère la mémoire */
	free(stack);
    free(status);

    return 0;
}

/* Trouve la prochaine arête e adjacente à current dans G tel que e 
 * n'est pas un isthme */
int pick_next(const int current, struct node_t * g, int n)
{
    int i = 0;
    int next = -1;
    
    /* Si le noeud n'a qu'une seule arête */
	if(g[current].degree == 1)
	{
		next = g[current].neighbours[0]->id;
		
		/* Visiter l'arête */
		graph_darken_edge(&g[current], g[current].neighbours[0], 0);
	}
	else
	{
		/* Pour chaque voisins du noeud ... 
		 * La première arête qui n'est pas un isthme est définitivement
		 * visité et est renvoyée */
		for(i = 0; i < g[current].degree; ++i)
		{
			next = g[current].neighbours[i]->id;
			
			graph_darken_edge(&g[current], g[current].neighbours[i], i);
			
			if(is_not_isthme_dfs(&g[next], n, g[current].id))
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
	int output = 1;

	if(argc == 2)
	{
		output = 0;
	}
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

			/* printf("Application de l'algorithme de fleury sur G(%d, %d) ...\n", n, m); */
			
			size = fleury(g, n, out);
			
			if(size == m)
			{
				/* printf("Solution trouvée !\n"); */
				
				/* On ajoute 1, c'est un peu bizarre mais le sujet dit que
				 * c'est numéroté de 1 à n, alors bon ... */
				if(output)
				{
					for(i = 0; i <= size; ++i)
						printf("%d ", out[i] + 1);
					printf("\n");
				}
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
