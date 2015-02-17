#ifndef BRAVE_GENETIC_ALGORITHM
#define BRAVE_GENETIC_ALGORITHM


#include <iostream>
#include <map>
#include <random>
#include <tuple>
#include <vector>
#include <stdexcept>

template <class C>
class GeneticAlgorithm {
	
public:

	GeneticAlgorithm(void) {
		this->mutation_rate = 0.1;
		this->population_size = 100;
		this->crossover_rate = 1.0;
	}

	void set_mutation_rate(float r) {
		if (r < 0 || r > 1) {
			throw std::invalid_argument("Received invalid argument");
		}
		this->mutation_rate = r;
	}

	void set_crossover_rate(float r) {
		if (r < 0 || r > 1) {
			throw std::invalid_argument("Received invalid argument");
		}
		this->crossover_rate = r;
	}

	void set_population_size(int n) {
		if (n <= 0) {
			throw std::invalid_argument("Received invalid argument");
		}
		this->population_size = n;
	}

	void initialize(void) {
		for (int i = 0; i < this->population_size; i++) {
			this->population.push_back(this->gen_random());
		}
	}

	void run(int generations) {
		this->initialize();

		if (generations < 0) {
			throw std::invalid_argument("generations must be >= 0");
		}

		for (int i = 0; i < generations; i++) {
			this->step_once();
		}

		std::cout << "Final population after " << generations << " generations:" << std::endl;
		for (C& elem : this->population) {
			std::cout << elem << std::endl;
		}
	}

	void step_once(void) {
		std::vector<C> parent_pool,
			child_pool,
			mutated_child_pool;

		// assign fitness values for each
		std::cout << "Getting fitness values..." << std::endl;
		std::map<C, float> fitness_values;
		float fitness_total = 0;
		for (C& chrom : this->population) {
			float fitness = this->fitness_function(chrom);
			std::cout << "fitness(" << chrom << ") = " << fitness << std::endl;
			fitness_values[chrom] = fitness;
			fitness_total += fitness;
		}

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator (seed);
		std::uniform_real_distribution<float> uniform_dist(0, fitness_total);

		// create parent pool
		std::cout << "Creating parent pool (roulette)..." << std::endl;
		while (parent_pool.size() < this->population_size) {
			// generate random number
			float r = uniform_dist(generator);
			for (C& chrom : this->population) {
				r -= fitness_values[chrom];
				if (r <= 0) {
					std::cout << "Selecting chromosome " << chrom << " for parent pool" << std::endl;
					parent_pool.push_back(chrom);
					break;
				}
			}
		}

		// create the child pool
		std::cout << "Creating the child pool (crossover)..." << std::endl;
		while (child_pool.size() < this->population_size) {
			C p1 = parent_pool[rand() % this->population.size()];
			C p2 = parent_pool[rand() % this->population.size()];

			std::tuple<C, C> children = this->crossover(p1, p2);
			C c1,
			  c2;

			c1 = std::get<0>(children);
			c2 = std::get<1>(children);

			child_pool.push_back(c1);
			child_pool.push_back(c2);
		}

		// create the mutated child pool
		std::cout << "Mutating..." << std::endl;
		for (C& chrom : child_pool) {
			C mutated;
			float r = std::uniform_real_distribution<float>(0, 1)(generator);
			if (r < this->mutation_rate) {
				mutated = this->mutate(chrom);
				std::cout << chrom << " -> " << mutated << std::endl;
			} else {
				mutated = chrom;
				std::cout << chrom << std::endl;
			}
			mutated_child_pool.push_back(mutated);
		}

		this->population = mutated_child_pool;
	}

	virtual std::tuple<C, C> crossover(C& chrom1, C& chrom2) = 0;
	virtual int gen_random(void) = 0;
	virtual float fitness_function(C& chrom) = 0;
	virtual C mutate(C& chrom) = 0;
	virtual bool should_stop(void) = 0;

private:
	std::vector<C> population;

	float mutation_rate,
		  crossover_rate;
	int population_size;
};

#endif
