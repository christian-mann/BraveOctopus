#ifndef BRAVE_TRIVIAL_GENETIC_ALGORITHM
#define BRAVE_TRIVIAL_GENETIC_ALGORITHM

#include "GeneticAlgorithm.cpp"

class TrivialGeneticAlgorithm : public GeneticAlgorithm<int> {
	std::tuple<int, int> crossover(int& chrom1, int& chrom2) {
		return std::tuple<int, int>(chrom1, chrom2);
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
