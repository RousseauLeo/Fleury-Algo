#ifndef GRAPH_H
#define GRAPH_H

struct edge_t {int u; int v;};

struct node_t
{
	int id;
	int * neighbours; 
	int degree;
};

void graph_create_from_list (struct node_t ** gp, int n, int m, struct edge_t * e, int * count);

void graph_destroy(struct node_t ** gp, int n);

void graph_dfs(struct node_t * g, int n, void process(struct node_t *g, int x));

#endif
