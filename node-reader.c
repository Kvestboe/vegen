#include "codec.h"
#include <string.h>

Node *read_nodes(const char *path, int *n) {
	FILE *f = fopen(path, "r");
	fscanf(f, "%d", n);

	Node *nodes = malloc(sizeof(Node) * *n);
	for (int i = 0; i < *n; i++) {
		int id;
		double lat, lon;
		fscanf(f, "%d %lf %lf", &id, &lat, &lon);
		nodes[id].lat = lat;
		nodes[id].lon = lon;
	}
	fclose(f);
	return nodes;
}

void read_edges(const char *path, int *m, int **fra, int **til, int **tid) {
	FILE *f = fopen(path, "r");
	fscanf(f, "%d", m);

	*fra = malloc(sizeof(int) * *m);
	*til = malloc(sizeof(int) * *m);
	*tid = malloc(sizeof(int) * *m);
	for (int i = 0; i < *m; i++)
		fscanf(f, "%d %d %d %*d %*d", &(*fra)[i], &(*til)[i], &(*tid)[i]);
	fclose(f);
}

Graph build_graph(Node *nodes, int n, int m, int *fra, int *til, int *tid) {
	Graph g;
	g.n = n;
	g.nodes = nodes;
	g.first = calloc(n + 1, sizeof(int));
	g.edges = malloc(sizeof(Edge) * m);

	for (int i = 0; i < m; i++) g.first[fra[i] + 1]++;
	for (int i = 0; i < n; i++) g.first[i + 1] += g.first[i];

	int *next = malloc(sizeof(int) * n);
	memcpy(next, g.first, sizeof(int) * n);
	for (int i = 0; i < m; i++) {
		int k = next[fra[i]]++;
		g.edges[k].til = til[i];
		g.edges[k].tid = tid[i];
	}
	free(next);
	return g;
}

unsigned char *read_poi(const char *path, int n, char ***names) {
	FILE *f = fopen(path, "r");
	unsigned char *code = calloc(n, 1);
	*names = calloc(n, sizeof(char *));

	char line[1024], name[1024];
	int id, c;
	fgets(line, sizeof line, f);
	while (fgets(line, sizeof line, f)) {
		sscanf(line, "%d %d \"%[^\"]", &id, &c, name);
		code[id] = c;
		(*names)[id] = strdup(name);
	}
	fclose(f);
	return code;
}
