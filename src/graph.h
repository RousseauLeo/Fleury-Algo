#ifndef GRAPH_H
#define GRAPH_H

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
    int * neighbours;
    int degree;
};

/*
 * Permet de créer un graphe à partir d'une liste de sommets et de leurs
 * voisins
 */
void graph_create_from_list (struct node_t * g, int n, int m, struct edge_t * e, int * count);

/* Détruit le graphe et libère la mémoire */
void graph_destroy (struct node_t * g, int n);

/* Noircir une arête (la rendre inutilisable) */
void graph_darken_edge(struct node_t * a, struct node_t * b);

/* Eclaircir une arête (la rendre visitable) */
void graph_undarken_edge(struct node_t * a, struct node_t * b);

/* 
 * Permet de parcourir le graphe en profondeur. La fonction process
 * donnée par l'utilisateur sera appelé pour chaque noeuds
 *
 * Renvoie le nombre de composantes connexes dans g
 */
int graph_dfs_func (struct node_t * g, int n, void process(struct node_t *g, int x));

/*
 * Renvoie le nombre de composantes connexes dans g
 */
int graph_dfs_count (struct node_t * g, int n);

/*
 * Affiche le graphe
 */
int graph_dfs_display (struct node_t * g, int n);

#endif
