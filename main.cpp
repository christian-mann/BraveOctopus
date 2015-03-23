#include "GeneticAlgorithm.cpp"
#include "TrivialGeneticAlgorithm.cpp"
#include "Graph.cpp"
#include "VertexCoverGA.cpp"

#define GRAPH_SIZE 100

int main(int argc, char **argv) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed);

	Graph g(GRAPH_SIZE);

	for (int i = 0; i < GRAPH_SIZE * 2; i++) {
		g.add_edge(i % GRAPH_SIZE, rand() % GRAPH_SIZE);
	}

	if (argc >= 2 && !strcmp(argv[1], "vertexcover")) {
		VertexCoverGeneticAlgorithm vga(g);
		vga.set_population_size(100);
		vga.run(400);
	} else {
		TrivialGeneticAlgorithm tga;
		tga.run(10);
	}
}
