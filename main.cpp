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

	parser.add_option("-m", "--mutation-rate")
		.set_default("0.01")
		.type("float")
		.dest("mutation_rate");
	parser.add_option("--target")
		.type("float")
		.dest("target");
	parser.add_option("--graph-size")
		.type("int")
		.set_default("100");
	parser.add_option("--algorithm-type")
		.type("string")
		.set_default("ga");
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
	parser.add_option("--pool-size")
		.type("int")
		.set_default("100");
	parser.add_option("--graph-file")
		.type("string");
	auto args = parser.parse_args(argc, argv);

	int generations = (int) args.get("generations");
	float mutation_rate = (float) args.get("mutation_rate");
	string algorithm_type = (string) args.get("algorithm_type");
	int pool_size = (int) args.get("pool_size");


	Graph g;
	int graph_size;
	if (args.is_set("graph_file")) {
		g = Graph::deserialize((string) args.get("graph_file"));
		graph_size = g.size();
	} else if (args.is_set("graph_size")) {
		// random generated graph

		graph_size = (int) args.get("graph_size");
		g = Graph(graph_size);


		/*
		// make good solution
		for (int v2 = 0; v2 < graph_size; v2++) {
			g.add_edge(v2, rand() % 50);
		}

		// hide it
		for (int v2 = 0; v2 < graph_size; v2++) {
			int v1 = rand() % 50 + 10;
			g.add_edge(v1, v2);
		}
		*/

		for (int i = 0; i < graph_size * graph_size * 0.002; i++) {
			g.add_edge(rand() % (graph_size - 1), rand() % (graph_size - 1));
		}
	} else {
		cerr << "I need one of graph-size or graph-file" << endl;
		exit(1);
	}

	// run algorithm
	if (algorithm_type == "ga") {
		VertexCoverGeneticAlgorithm vga(g);
		vga.set_mutation_rate(mutation_rate);
		vga.set_population_size(pool_size);
		vga.run(generations);
	} else if (algorithm_type == "sa") {
		VertexCoverSimulatedAnnealing vsa(g);
		vsa.set_temperature(2);
		vsa.set_iterations(1000);
		vsa.set_mutation_rate(mutation_rate);

		if (args.is_set("target")) {
			vsa.set_goal((float) args.get("target"));
		}
		vsa.run(generations);
	} else if (algorithm_type == "hc" ) {
		VertexCoverHillClimbing vhc(g);
		vhc.run(generations);
	} else if (algorithm_type == "greedy" ) {
		GreedySolver greed(g);
		VertexCoverChrom chrom = greed.solve();
		cout << chrom << endl;
	} else {
		cerr << "Unrecognized algorithm type" << endl;
	}

	g.serialize("graph.txt");

	cout << "Total fitness evaluations: " << VertexCoverChrom::total_fitness_evaluations << endl;
}
