#include "codec.h"

void heap_init(Heap *h, int cap) {
	h->size = 0;
	h->cap = cap;
	h->a = malloc(sizeof(HeapItem) * cap);
}

void heap_free(Heap *h) {
	free(h->a);
}

static void swap(HeapItem *a, HeapItem *b) {
	HeapItem t = *a;
	*a = *b;
	*b = t;
}

void heap_push(Heap *h, int node, int pri) {
	if (h->size == h->cap) {
		h->cap *= 2;
		h->a = realloc(h->a, sizeof(HeapItem) * h->cap);
	}

	int i = h->size++;
	h->a[i].node = node;
	h->a[i].pri = pri;

	while (i > 0) {
		int p = (i - 1) / 2;
		if (h->a[p].pri <= h->a[i].pri) break;
		swap(&h->a[p], &h->a[i]);
		i = p;
	}
}

HeapItem heap_pop(Heap *h) {
	HeapItem top = h->a[0];
	h->a[0] = h->a[--h->size];

	int i = 0;
	for (;;) {
		int l = 2 * i + 1;
		int r = l + 1;
		int min = i;
		if (l < h->size && h->a[l].pri < h->a[min].pri) min = l;
		if (r < h->size && h->a[r].pri < h->a[min].pri) min = r;
		if (min == i) break;
		swap(&h->a[i], &h->a[min]);
		i = min;
	}
	return top;
}
