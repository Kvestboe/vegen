#include "codec.h"

int shortest_path(Graph *g, int start, int goal, Landmarks *lm, int *popped) {
	Node *nodes = g->nodes;
	for (int i = 0; i < g->n; i++) {
		nodes[i].dist = UENDELIG;
		nodes[i].prev = -1;
		nodes[i].est = -1;
	}

	Heap h;
	heap_init(&h, 1024);

	nodes[start].dist = 0;
	nodes[start].est = lm ? estimate(lm, start, goal) : 0;
	heap_push(&h, start, nodes[start].est);
	*popped = 0;

	while (h.size > 0) {
		HeapItem it = heap_pop(&h);
		int u = it.node;
		if (it.pri > nodes[u].dist + nodes[u].est) continue;

		(*popped)++;
		if (u == goal) break;

		for (int e = g->first[u]; e < g->first[u + 1]; e++) {
			int v = g->edges[e].til;
			int ny = nodes[u].dist + g->edges[e].tid;
			if (ny < nodes[v].dist) {
				nodes[v].dist = ny;
				nodes[v].prev = u;
				if (nodes[v].est < 0) nodes[v].est = lm ? estimate(lm, v, goal) : 0;
				heap_push(&h, v, ny + nodes[v].est);
			}
		}
	}
	heap_free(&h);

	return goal < 0 ? 0 : nodes[goal].dist;
}

int find_poi(Graph *g, int start, unsigned char *code, int mask, int *found, int max) {
	Node *nodes = g->nodes;
	for (int i = 0; i < g->n; i++) {
		nodes[i].dist = UENDELIG;
		nodes[i].prev = -1;
	}

	Heap h;
	heap_init(&h, 1024);
	nodes[start].dist = 0;
	heap_push(&h, start, 0);
	int cnt = 0;

	while (h.size > 0 && cnt < max) {
		HeapItem it = heap_pop(&h);
		int u = it.node;
		if (it.pri > nodes[u].dist) continue;

		if (code[u] & mask) found[cnt++] = u;

		for (int e = g->first[u]; e < g->first[u + 1]; e++) {
			int v = g->edges[e].til;
			int ny = nodes[u].dist + g->edges[e].tid;
			if (ny < nodes[v].dist) {
				nodes[v].dist = ny;
				nodes[v].prev = u;
				heap_push(&h, v, ny);
			}
		}
	}
	heap_free(&h);
	return cnt;
}
