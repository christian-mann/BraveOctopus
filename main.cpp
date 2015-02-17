#include "GeneticAlgorithm.cpp"
#include "TrivialGeneticAlgorithm.cpp"

int main() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed);

	TrivialGeneticAlgorithm tga;

	tga.set_population_size(10);
	tga.run(9);
}
