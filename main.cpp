#include "GeneticAlgorithm.cpp"
#include "TrivialGeneticAlgorithm.cpp"
#include "Graph.cpp"
#include "VertexCoverGA.cpp"

#include "cpp-argparse/OptionParser.cpp"

using optparse::OptionParser;

int main(int argc, char **argv) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed);


	// option parsing
	OptionParser parser = OptionParser();

	parser.add_option("-m", "--mutation-rate")
		.set_default("0.1")
		.type("float")
		.dest("mutation_rate");
	parser.add_option("--graph-size")
		.type("int")
		.set_default("100");
	parser.add_option("-g", "--generations")
		.set_default("100")
		.type("int");
	char const* const crossover_types[] = {"uniform", "1point"};
	parser.add_option("--crossover-type")
		.type("choices")
		.choices(&crossover_types[0], &crossover_types[2])
		.set_default("uniform");
	char const* const selection_types[] = {"rank", "roulette", "tournament"};
	parser.add_option("--selection-type")
		.type("choices")
		.choices(&selection_types[0], &selection_types[3])
		.set_default("roulette");
	auto args = parser.parse_args(argc, argv);

	int graph_size = (int) args.get("graph_size");
	int generations = (int) args.get("generations");


	Graph g(graph_size);
	// make perfect solution
	for (int j = 0; j < graph_size; j++) {
		g.add_edge(0, j);
	}

	/*
	// make good solution
	for (int i = 0; i < 3; i++) {
		for (int j = (graph_size * i) / 5; j < (graph_size * (i+1))/5; j++) {
			g.add_edge(i, j);
		}
	}
	*/

	// hide it
	for (int i = 0; i < graph_size; i++) {
		g.add_edge(i % graph_size, rand() % graph_size);
	}


	// run algorithm
	VertexCoverGeneticAlgorithm vga(g);
	vga.set_population_size(100);
	vga.run(generations);
}
