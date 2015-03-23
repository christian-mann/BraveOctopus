#ifndef BRAVE_TRIVIAL_GENETIC_ALGORITHM
#define BRAVE_TRIVIAL_GENETIC_ALGORITHM

#include "GeneticAlgorithm.cpp"

#include <random>

class TrivialGeneticAlgorithm : public GeneticAlgorithm<int> {
	std::tuple<int, int> crossover(int& p1, int& p2) {
		int crossover_key = rand() % 1024;
		//std::cout << "crossover_key = " << std::hex << crossover_key << std::endl;

		int c1 = (p1 & crossover_key) | (p2 & ~crossover_key);
		int c2 = (p1 & ~crossover_key) | (p2 & crossover_key);
		return std::tuple<int, int>(c1, c2);
	}

	int gen_random(void) {
		return rand() % 100;
	}

	float fitness_function(int& chrom) {
		return (float) chrom;
	}

	int mutate(int& chrom) {
		return chrom - 1;
	}

	bool should_stop(void) {
		return false;
	}
};

#endif
