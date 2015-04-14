#ifndef BRAVE_GENETIC_ALGORITHM
#define BRAVE_GENETIC_ALGORITHM


#include <iostream>
#include <map>
#include <random>
#include <tuple>
#include <vector>
#include <stdexcept>

#include "Chromosome.cpp"

using namespace std;

template <class C>
// C extends Chromosome
class GeneticAlgorithm {
	
public:

	GeneticAlgorithm(void) {
		this->mutation_rate = 0.1;
		this->population_size = 100;
		this->crossover_rate = 1.0;
	}

	void set_mutation_rate(float r) {
		if (r < 0 || r > 1) {
			throw invalid_argument("Received invalid argument");
		}
		this->mutation_rate = r;
	}

	void set_crossover_rate(float r) {
		if (r < 0 || r > 1) {
			throw invalid_argument("Received invalid argument");
		}
		this->crossover_rate = r;
	}

	void set_population_size(int n) {
		if (n <= 0) {
			throw invalid_argument("Received invalid argument");
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
			throw invalid_argument("generations must be >= 0");
		}

		for (int i = 0; i < generations; i++) {
			this->step_once();
			int num_viable = 0,
				num_inviable = 0;
			for (C& chrom : this->population) {
				if (chrom.is_viable()) {
					num_viable++;
				} else {
					num_inviable++;
				}
			}
			cout << num_viable << " viable; " << num_inviable << " inviable" << endl;
		}

		cout << "Best element after " << generations << " generations: " << endl;
		vector<C> best;
		this->absolute_selection(this->population, best, 1);
		cout << best[0] << endl;
		//for (C& elem : this->population) {
			//cout << elem << endl;
		//}
	}

	void step_once(void) {
		vector<C>
			elite_pool,
			parent_pool,
			child_pool,
			mutated_child_pool;

		// output info about the generation
		cout << "Getting fitness values..." << endl;
		float fitness_total = 0;
		float fitness_max = 0;
		for (C& chrom : this->population) {
			float fitness = chrom.fitness();
			cout << fitness << " ";
			fitness_total += fitness;
			fitness_max = max(fitness_max, fitness);
		}
		cout << endl;
		cout << "Average fitness: " << fitness_total / population.size() << endl;
		cout << "Maximum fitness: " << fitness_max << endl;
		
		// create the elite pool
		this->absolute_selection(this->population, elite_pool, 2);

		for (C& chrom : elite_pool) {
			cout << "elite: " << chrom.fitness() << endl;
		}
		cout << endl;

		// create the parent pool
		this->tournament_selection(this->population, parent_pool);

		// create the child pool
		//cout << "Creating the child pool (crossover)..." << endl;
		while (child_pool.size() < this->population_size) {
			C& p1 = parent_pool[rand() % this->population.size()];
			C& p2 = parent_pool[rand() % this->population.size()];

			C c1 = p1.crossover(p2);
			C c2 = p2.crossover(p1);

			child_pool.push_back(c1);
			child_pool.push_back(c2);
		}

		// create the mutated child pool
		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator (seed);
		//cout << "Mutating..." << endl;
		for (C& chrom : child_pool) {
			C mutated = chrom.mutate(this->mutation_rate);
			mutated_child_pool.push_back(mutated);
		}

		this->population.clear();
		// put most of the new in
		this->population = mutated_child_pool;

		// put the elite elements back in
		for (int i = 0; i < elite_pool.size(); i++) {
			this->population[i] = elite_pool[i];
		}
	}

	virtual C gen_random(void) = 0;
	virtual bool should_stop(void) = 0;

private:
	vector<C> population;

	float mutation_rate,
		  crossover_rate;
	int population_size;

	void roulette_selection(vector<C>& population, vector<C> &parent_pool) {
		// assign fitness values for each
		cout << "Getting fitness values..." << endl;
		vector<pair<C, float>> fitness_values;
		float fitness_total = 0;
		for (C& chrom : this->population) {
			float fitness = chrom.fitness();
			fitness_total += fitness;
		}
		cout << endl;

		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator (seed);
		uniform_real_distribution<float> uniform_dist(0, fitness_total);
		// create parent pool
		cout << "Creating parent pool (roulette)..." << endl;
		while (parent_pool.size() < this->population_size) {
			// generate random number
			float r = uniform_dist(generator);
			for (C& chrom : this->population) {
				r -= chrom.fitness();
				if (r <= 0) {
					parent_pool.push_back(chrom);
					break;
				}
			}
		}
	}

	void rank_selection(vector<C>& population, vector<C>& parent_pool) {
		// assign fitness values for each
		cout << "Getting fitness values..." << endl;
		vector<pair<float, C>> fitness_values;
		float fitness_total;
		float fitness_max;
		for (C& chrom : this->population) {
			float fitness = chrom.fitness();
			fitness_total += fitness;
			fitness_max = max(fitness_max, fitness);
			cout << fitness << " ";
			fitness_values.push_back(pair<float, C>(fitness, chrom));
		}
		cout << endl;
		cout << "Average fitness: " << fitness_total / population.size() << endl;
		cout << "Maximum fitness: " << fitness_max << endl;

		parent_pool.insert(parent_pool.begin(), population.begin(), population.end());

		sort(parent_pool.begin(), parent_pool.end());
	}

	void absolute_selection(vector<C> &population, vector<C> &pool, int max_elements = -1) {
		if (max_elements == -1) {
			max_elements = population.size();
		}
		// assign fitness values for each
		vector<pair<float, C>> fitness_values;
		for (C& chrom : this->population) {
			float fitness = chrom.fitness();
			fitness_values.push_back(pair<float, C>(-fitness, chrom));
		}

		sort(fitness_values.begin(), fitness_values.end());

		cout << "fitness_values.size() == " << fitness_values.size() << endl;

		cout << "pushing back...";
		for (int i = 0; i < 2; i++) {
			pool.push_back(get<1>(fitness_values[i]));
		}
		cout << "pushed back" << endl;

		//pool.insert(pool.begin(), population.begin(), population.begin() + max_elements);

	}

	void tournament_selection(vector<C> &population, vector<C>& pool) {
		while (pool.size() < population.size()) {
			// take two random elements
			C &p1 = population[rand() % population.size()];
			C &p2 = population[rand() % population.size()];

			C &c1 = p1;

			// pick the better one
			if (p1.fitness() >= p2.fitness()) {
				if (rand() % 100 < 75) {
					c1 = p1;
				} else {
					c1 = p2;
				}
			} else {
				if (rand() % 100 < 75) {
					c1 = p2;
				} else {
					c1 = p1;
				}
			}
			pool.push_back(c1);
		}
	}
};

#endif
