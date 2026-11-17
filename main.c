#include "codec.h"
#include <string.h>
#include <time.h>

static void print_time(int hundredths) {
	int s = hundredths / 100;
	printf("%02d:%02d:%02d", s / 3600, (s / 60) % 60, s % 60);
}

static void print_result(const char *algo, int d, double secs, int popped) {
	printf("%s: kjøretid ", algo);
	print_time(d);
	printf(", beregningstid %.3f s, %d noder plukket ut av køen\n", secs, popped);
}

int main(int argc, char **argv) {
	char *dir = argv[1], *mode = argv[2];
	char path[1024];
	int n, m, *fra, *til, *tid;

	snprintf(path, sizeof path, "%s/noder.txt", dir);
	Node *nodes = read_nodes(path, &n);
	snprintf(path, sizeof path, "%s/kanter.txt", dir);
	read_edges(path, &m, &fra, &til, &tid);
	Graph g = build_graph(nodes, n, m, fra, til, tid);

	snprintf(path, sizeof path, "%s/landemerker.bin", dir);

	if (strcmp(mode, "pre") == 0) {
		Graph rg = build_graph(nodes, n, m, til, fra, tid);
		int k = argc - 3;
		int *ids = malloc(sizeof(int) * k);
		for (int i = 0; i < k; i++) ids[i] = atoi(argv[3 + i]);
		preprocess(&g, &rg, ids, k, path);
	}

	if (strcmp(mode, "rute") == 0) {
		int start = atoi(argv[3]), goal = atoi(argv[4]);
		int popped;

		clock_t t0 = clock();
		int d = shortest_path(&g, start, goal, NULL, &popped);
		print_result("Dijkstra", d, (double)(clock() - t0) / CLOCKS_PER_SEC, popped);
		write_route(&g, goal, d, "rute.txt", "rute.html");

		Landmarks *lm = load_landmarks(path, n);
		t0 = clock();
		d = shortest_path(&g, start, goal, lm, &popped);
		print_result("ALT", d, (double)(clock() - t0) / CLOCKS_PER_SEC, popped);
	}

	if (strcmp(mode, "poi") == 0) {
		int start = atoi(argv[3]), mask = atoi(argv[4]);
		char **names;
		snprintf(path, sizeof path, "%s/interessepkt.txt", dir);
		unsigned char *code = read_poi(path, n, &names);

		int found[4];
		int cnt = find_poi(&g, start, code, mask, found, 4);
		for (int i = 0; i < cnt; i++) {
			printf("%d. %s (node %d) ", i + 1, names[found[i]], found[i]);
			print_time(nodes[found[i]].dist);
			printf("\n");
		}
		write_poi_map(&g, start, found, cnt, names, "poi.html");
	}
	return 0;
}
