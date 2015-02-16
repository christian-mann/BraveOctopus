#include "GeneticAlgorithm.hpp"

#include <tuple>
#include <vector>
#include <stdexcept>

template <class C>
void GeneticAlgorithm<C>::set_mutation_rate(float r) {
	if (r < 0 || r > 1) {
		throw std::invalid_argument("Received invalid argument");
	}
	this->mutation_rate = r;
}

template <class C>
void GeneticAlgorithm<C>::set_crossover_rate(float r) {
	if (r < 0 || r > 1) {
		throw std::invalid_argument("Received invalid argument");
	}
	this->crossover_rate = r;
}

template <class C>
void GeneticAlgorithm<C>::set_population_size(float n) {
	if (n < 0) {
		throw std::invalid_argument("Received invalid argument");
	}
	this->population_size = n;
}

template <class C>
void GeneticAlgorithm<C>::initialize(void) {
	for (int i = 0; i < this->population_size; i++) {
		this->population.push_back(this->gen_random());
	}
}
