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

int compare (void const *a, void const *b)
{
   const struct node_t *pa = (struct node_t *)a;
   const struct node_t *pb = (struct node_t *)b;

   /* Evaluer et retourner l'etat de l'evaluation (tri croissant) */
   return pa->id - pb->id;
}

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
    
    /* Tri des voisins de chaque sommet du graphe
     * Cela permet d'augmenter les performances de l'algorithme de 
     * fleury dans certains cas */
    for(i = 0; i < n; ++i)
		qsort(&g[i].neighbours[0], g[i].degree, sizeof(struct node_t*), compare);
    
    return 0;
}

/* Créer un graphe à partir des données lues sur l'entré standard */
int graph_read_from_std (struct node_t ** gp, int * np, int * mp)
{
	int n = 0, m = 0, s = 0, c = 0, i = 0;
	int count;
	int * buf;
	int * ned;
	int trash;
	int ret = 0;
	
	struct edge_t * e = 0;
	
	trash = scanf("%d", &n);
	
	buf = (int *)malloc(n * sizeof(int));
	ned = (int *)malloc(n * sizeof(int));
	e   = (struct edge_t *)malloc(n * n * sizeof(struct edge_t));
	*gp	= (struct node_t *)malloc(n * sizeof(struct node_t));
	
	for(c = 0; c < n; c++)
	{
		count = 0;
		
		trash = scanf("%d" ,&s);
		
		do {
			m++;
			/* On retire 1, c'est un peu bizarre mais le sujet dit que
			 * c'est numéroté de 1 à n, alors bon ... */
			buf[count++] = s - 1;
			trash = scanf("%d", &s);
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

/* Libère la mémoire utilisé pour le graphe */
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

/* Noircit une arête ; une arête noircit n'est plus visitable */
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

/* Eclair une arête ; une arête éclairée est visitable 
 * Note: cette fonction maintient les voisins triés */
void graph_undarken_edge(struct node_t * a, struct node_t * b)
{
	int i = 0;
    
    for(i = 0; i < a->degree; ++i)
    {
        if(b->id > a->neighbours[i]->id || i == a->degree - 1)
        {
            a->neighbours[a->degree++] = a->neighbours[i];
            a->neighbours[i] = b;
            break;
        }
    }

    for(i = 0; i < b->degree; ++i)
    {
        if(a->id > b->neighbours[i]->id || i == b->degree - 1)
        {
            b->neighbours[b->degree++] = b->neighbours[i];
            b->neighbours[i] = a;
            break;
        }
    }
}

/* Appel récursif pour le parfours en profondeur (Affichage) */
void __dfsDisplayRec (struct node_t * n, int * status)
{
    int i = 0;

    status[n->id] = 1;

    for(i = 0; i < n->degree; ++i)
        printf("%d -> %d\n", n->id, n->neighbours[i]->id);
    
	for(i = 0; i < n->degree; ++i)
	{
		/* Si le noeud est accessible */
		if(status[n->neighbours[i]->id] == 0)
			__dfsDisplayRec(n->neighbours[i], status);
	}
}

void graph_dfs_display (struct node_t * gorig, int n)
{
    int * status = (int *)malloc(n * sizeof(int));

	memset(status, 0, n * sizeof(int));
    
    printf("N = %d\n", n);
    
	__dfsDisplayRec(gorig, status);

    free(status);
}
