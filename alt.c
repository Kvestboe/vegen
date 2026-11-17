#include "codec.h"

void preprocess(Graph *g, Graph *rg, int *ids, int k, const char *path) {
	int n = g->n;
	int *from = malloc(sizeof(int) * (size_t)n * k);
	int *to = malloc(sizeof(int) * (size_t)n * k);
	int popped;

	for (int i = 0; i < k; i++) {
		shortest_path(g, ids[i], -1, NULL, &popped);
		for (int v = 0; v < n; v++) from[(size_t)v * k + i] = g->nodes[v].dist;

		shortest_path(rg, ids[i], -1, NULL, &popped);
		for (int v = 0; v < n; v++) to[(size_t)v * k + i] = rg->nodes[v].dist;
	}

	FILE *f = fopen(path, "wb");
	fwrite(&k, sizeof(int), 1, f);
	fwrite(from, sizeof(int), (size_t)n * k, f);
	fwrite(to, sizeof(int), (size_t)n * k, f);
	fclose(f);

	free(from);
	free(to);
}

Landmarks *load_landmarks(const char *path, int n) {
	FILE *f = fopen(path, "rb");
	Landmarks *lm = malloc(sizeof(Landmarks));

	fread(&lm->k, sizeof(int), 1, f);
	size_t size = (size_t)n * lm->k;
	lm->from = malloc(sizeof(int) * size);
	lm->to = malloc(sizeof(int) * size);
	fread(lm->from, sizeof(int), size, f);
	fread(lm->to, sizeof(int), size, f);

	fclose(f);
	return lm;
}

int estimate(Landmarks *lm, int v, int goal) {
	int k = lm->k, best = 0;
	int *fv = &lm->from[(size_t)v * k], *fg = &lm->from[(size_t)goal * k];
	int *tv = &lm->to[(size_t)v * k], *tg = &lm->to[(size_t)goal * k];

	for (int i = 0; i < k; i++) {
		if (fg[i] != UENDELIG && fv[i] != UENDELIG && fg[i] - fv[i] > best) best = fg[i] - fv[i];
		if (tv[i] != UENDELIG && tg[i] != UENDELIG && tv[i] - tg[i] > best) best = tv[i] - tg[i];
	}
	return best;
}
