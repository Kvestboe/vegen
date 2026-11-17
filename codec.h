#ifndef CODEC_H
#define CODEC_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define UENDELIG INT_MAX

typedef struct {
	double lat;
	double lon;
	int dist;
	int prev;
	int est;
} Node;

typedef struct {
	int til;
	int tid;
} Edge;

typedef struct {
	int n;
	Node *nodes;
	int *first;
	Edge *edges;
} Graph;

typedef struct {
	int k;
	int *from;
	int *to;
} Landmarks;

typedef struct {
	int node;
	int pri;
} HeapItem;

typedef struct {
	HeapItem *a;
	int size;
	int cap;
} Heap;

Node *read_nodes(const char *path, int *n);
void read_edges(const char *path, int *m, int **fra, int **til, int **tid);
Graph build_graph(Node *nodes, int n, int m, int *fra, int *til, int *tid);
unsigned char *read_poi(const char *path, int n, char ***names);

void heap_init(Heap *h, int cap);
void heap_free(Heap *h);
void heap_push(Heap *h, int node, int pri);
HeapItem heap_pop(Heap *h);

int shortest_path(Graph *g, int start, int goal, Landmarks *lm, int *popped);
int find_poi(Graph *g, int start, unsigned char *code, int mask, int *found, int max);

void preprocess(Graph *g, Graph *rg, int *ids, int k, const char *path);
Landmarks *load_landmarks(const char *path, int n);
int estimate(Landmarks *lm, int v, int goal);

void write_route(Graph *g, int goal, int time, const char *txt, const char *html);
void write_poi_map(Graph *g, int start, int *found, int cnt, char **names, const char *html);

#endif
