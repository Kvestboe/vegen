#include "codec.h"

Node *read_nodes(FILE *input, int node_count) {
	
	Node *output = malloc(sizeof(Node)*node_count);

	for (int i = 0; i < node_count; i++) {
		int id;
		double x, y;
		fscanf(input, "%d %lf %lf", &id, &x, &y);
		output[id].x = x;
		output[id].y = y;
	}

	return output;
}


