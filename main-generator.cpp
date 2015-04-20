#include "GeneticAlgorithm.cpp"
#include "SimulatedAnnealing.cpp"
#include "TrivialGeneticAlgorithm.cpp"
#include "GreedySolver.cpp"
#include "Graph.cpp"
#include "VertexCoverGA.cpp"

#include "cpp-argparse/OptionParser.cpp"

using optparse::OptionParser;

int main(int argc, char **argv) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed);


	// option parsing
	OptionParser parser = OptionParser();

	parser.add_option("--graph-size")
		.type("int")
		.set_default("100");
	parser.add_option("--output-file")
		.type("string");
	parser.add_option("--edge-count")
		.type("int");
	parser.add_option("--edge-chance")
		.type("float");
	auto args = parser.parse_args(argc, argv);

	default_random_engine generator (seed);
	uniform_real_distribution<float> uniform_dist(0, 1);

	Graph g;
	int graph_size;
	// random generated graph
	graph_size = (int) args.get("graph_size");
	g = Graph(graph_size);
	if (args.is_set("edge_chance")) {

		float edge_chance = (float) args.get("edge_chance");

		for (int v1 = 0; v1 < graph_size; v1++) {
			for (int v2 = 0; v2 < graph_size; v2++) {
				if (uniform_dist(generator) < edge_chance) {
					g.add_edge(rand() % (graph_size - 1), rand() % (graph_size - 1));
				}
			}
		}
	} else if (args.is_set("edge_count")) {
		int edge_count = (int) args.get("edge_count");
		int edges_to_place = edge_count;
		if (edges_to_place > graph_size * graph_size * 0.2) {
			throw "Place fewer edges pls";
		}
		while (edges_to_place > 0) {
			int v1 = rand() % graph_size;
			int v2 = rand() % graph_size;

			if (! g.has_edge(v1, v2)) {
				g.add_edge(v1, v2);
				edges_to_place--;
			}
		}
	}
	cout << "Done generating" << endl;

	g.serialize((string) args.get("output_file"));
}
