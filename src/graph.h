#ifndef GRAPH_H
#define GRAPH_H

struct edge {int u; int v;};

struct node
{
	int id;
	int * around; 
	int degree;
};

typedef struct node* Graph;


#endif
