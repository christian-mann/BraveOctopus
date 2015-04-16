#include "GeneticAlgorithm.cpp"
#include "SimulatedAnnealing.cpp"
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
	char const* const algorithm_types[] = {"ga", "sa", "hc"};
	parser.add_option("--algorithm-type")
		.type("choices")
		.choices(&algorithm_types[0], &algorithm_types[3])
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
	auto args = parser.parse_args(argc, argv);

	int graph_size = (int) args.get("graph_size");
	int generations = (int) args.get("generations");
	float mutation_rate = (float) args.get("mutation_rate");
	string algorithm_type = (string) args.get("algorithm_type");
	int pool_size = (int) args.get("pool_size");


	Graph g(graph_size);
	/*
	// make perfect solution
	for (int j = 0; j < graph_size; j++) {
		g.add_edge(0, j);
	}
	*/

	/*
	// make good solution
	for (int i = 0; i < 3; i++) {
		for (int j = (graph_size * i) / 5; j < (graph_size * (i+1))/5; j++) {
			g.add_edge(i, j);
		}
	}
	*/

	// random generated graph
	for (int i = 0; i < graph_size * graph_size * 0.01; i++) {
		g.add_edge(rand() % (graph_size - 10), rand() % (graph_size - 10));
	}


	// run algorithm
	if (algorithm_type == "ga") {
		VertexCoverGeneticAlgorithm vga(g);
		vga.set_mutation_rate(mutation_rate);
		vga.set_population_size(pool_size);
		vga.run(generations);
	} else if (algorithm_type == "sa") {
		VertexCoverSimulatedAnnealing vsa(g);
		vsa.set_temperature(10);
		vsa.set_iterations(1000);
		vsa.set_mutation_rate(mutation_rate);
		vsa.run(generations);
	} else if (algorithm_type == "hc" ) {
		VertexCoverHillClimbing vhc(g);
		vhc.run(generations);
	}

	cout << "Total fitness evaluations: " << VertexCoverChrom::total_fitness_evaluations << endl;
}
