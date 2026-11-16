#include <stdlib.h>
#include <stdio.h>

typedef struct {
	double x;
	double y;
} Node;

typedef struct {
	int franode;
	int tilnode;
	int kjøretid;
	int lengde;
	int fartsgrense; 
} Edge; 

Node *read_nodes(FILE *input, int node_count);


