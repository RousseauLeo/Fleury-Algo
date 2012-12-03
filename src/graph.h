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

#ifndef GRAPH_H
#define GRAPH_H

#define DBG printf("%s:%d\n",__FILE__,__LINE__);

/*
 * Le graphe est représenté par une liste de noeuds et le nombre de
 * noeuds présents dans cette liste.
 *
 * Toutes les fontions affectant l'ensemble du graphe prennent donc en
 * paramètre le couple (liste des noeuds, taille de la liste)
 */

/* structure de base pour représenter une arête */
struct edge_t {int u; int v;};

/*
 * Structure de base pour représenter un noeud du graphe.
 * Contient la liste des index des voisins.
 * Un index négatif signifie que le noeud n'est pas accessible.
 */

struct node_t
{
    int id;
    struct node_t ** neighbours;
    int degree;
};

/*
 * Permet de créer un graphe à partir d'une liste de sommets et de leurs
 * voisins.
 * 
 * Retourne -1 en cas d'erreur, 0 sinon
 */
int graph_create_from_list (struct node_t * g, int n, int m, struct edge_t * e, int * count);

int graph_read_from_std (struct node_t ** gp, int * np, int * mp);

/* Détruit le graphe et libère la mémoire */
void graph_destroy (struct node_t * g, int n);

/* Noircir une arête (la rendre inutilisable) */
void graph_darken_edge(struct node_t * a, struct node_t * b, int x);

/* Eclaircit une arête (la rendre inutilisable) */
void graph_undarken_edge(struct node_t * a, struct node_t * b);

/* Affiche le graphe */
void graph_dfs_display (struct node_t * g, int n);

#endif
